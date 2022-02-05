#搭建Kubernetes集群

#configure static ip address
on host
gen new mac addr for vm in vmware-> virtual machine settings -> network adapter -> advanced
edit C:\ProgramData\VMware\vmnetdchp.conf
add 
```
host kube-master {
    hardware ethernet 00:50:56:35:81:8E;
    fixed-address 192.168.174.131;
}
```
restart vmware service

#install kubernetes utils
use proxy
export http_proxy=192.168.174.1:7890
export https_proxy=192.168.174.1:7890

apt-get install -y apt-transport-https ca-certificates curl

Download the Google Cloud public signing key
curl -fsSLo /usr/share/keyrings/kubernetes-archive-keyring.gpg https://packages.cloud.google.com/apt/doc/apt-key.gpg

Add the Kubernetes apt repository:  
echo "deb [signed-by=/usr/share/keyrings/kubernetes-archive-keyring.gpg] https://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list

apt use proxy:
echo 'Acquire::http::Proxy "http://192.168.174.1:7890";' > /etc/apt/apt.conf.d/apt_proxy.conf

install utils (apt can't use http_proxy env var)
apt update
apt-get install -y kubelet kubeadm kubectl
apt-mark hold kubelet kubeadm kubectl

#change docker cgroup driver
vim /etc/docker/daemon.json
```json
{
  "exec-opts": ["native.cgroupdriver=systemd"]
}
```
systemctl restart docker

#turn off swap
swapoff -a

#check kubelet log
journalctl -xeu kubelet

#network block
ufw status
ufw disable
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -F
route add default gw 192.168.119.1

#清理
kubeadm reset
rm -rf ~/.kube /etc/kubernetes
service kubelet stop

#kubeadm命令行
kubeadm init --pod-network-cidr 172.16.0.0/16
kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml

#generate init config
kubeadm config print init-defaults --kubeconfig ClusterConfiguration > kubeadm.yml

modify localAPIEndpoint->advertiseAddress
change nodeRegistration->name
change imageRepository if needed (registry.aliyuncs.com/google_containers)
change kubernetesVersion if needed

add podSubnet: "192.168.0.0/16" after serviceSubnet, don't conflict with virtual machine host network cidr

kubeadm init --config=kubeadm.yml

#use kubelet 
export KUBECONFIG=/etc/kubernetes/admin.conf

#获取join命令
kubeadm token create --print-join-command

#slave加入
kubeadm join xxx

#taint
展示
kubectl get nodes -o json | grep taint
删除
kubectl taint nodes master node-role.kubernetes.io/master=value1:NoSchedule-

#清除flannel
kubeadm reset
service kubelet stop 
service docker stop 
rm -rf /var/lib/cni/  /var/lib/kubelet/* /etc/cni/
rm -rf .kube /etc/kubernetes
ifconfig cni0 down
ip link delete cni0
ifconfig flannel.1 down
ip link delete flannel.1
ifconfig docker0 down
service docker start

#modify docker image registry
vim /etc/docker/daemon.json
{
"registry-mirrors": ["https://ustc-edu-cn.mirror.aliyuncs.com"]
}
service docker restart

#docker pull proxy
mkdir -p /etc/systemd/system/docker.service.d
touch /etc/systemd/system/docker.service.d/proxy.conf
```
[Service]
Environment="HTTP_PROXY=192.168.119.1:1080/"
Environment="HTTPS_PROXY=http://proxy.example.com:8080/"
Environment="NO_PROXY=localhost,127.0.0.1,.example.com"
```

#pulling google docker image inside wall
gcr.io/google-samples/node-hello:1.0
docker pull anjia0532/google-samples.node-hello:1.0
docker tag anjia0532/google-samples.node-hello:1.0 gcr.io/google-samples/node-hello:1.0

#saving docker image
docker save -onode-hello.tar gcr.io/google-samples/node-hello
docker load < node-hello.tar

#guestbook
https://kubernetes.io/docs/tutorials/stateless-application/guestbook/
if can't pull img from docker.io, change redis leader image to "bitnami/redis:6.0.5", add env var
```yaml
containers:
    - name: leader
      image: "bitnami/redis:6.0.5"
      resources:
        requests:
          cpu: 100m
          memory: 100Mi
      ports:
        - containerPort: 6379
      env:
      - name: ALLOW_EMPTY_PASSWORD
        value: "yes"
```
kubectl expose deployment frontend --type=LoadBalancer --name=frontend

#deploy stateful set
https://kubernetes.io/docs/tutorials/stateful-application/basic-stateful-set/
create 2 pv
```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: task-pv-volume1
  labels:
    type: local
spec:
  capacity:
    storage: 3Gi
  accessModes:
    - ReadWriteOnce
  hostPath:
    path: "/mnt/data"
---
apiVersion: v1
kind: PersistentVolume
metadata:
  name: task-pv-volume2
  labels:
    type: local
spec:
  capacity:
    storage: 1Gi
  accessModes:
    - ReadWriteOnce
  hostPath:
    path: "/mnt/data"
```
create sts with the provided web.yaml
