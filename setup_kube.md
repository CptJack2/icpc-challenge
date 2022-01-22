#搭建Kubernetes集群

##清理
kubeadm reset
rm -rf .kube /etc/kubernetes
service kubelet stop

##kubeadm命令行
kubeadm init --pod-network-cidr 172.16.0.0/16
kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml

#generate init config
kubeadm config print init-defaults --kubeconfig ClusterConfiguration > kubeadm.yml

modify localAPIEndpoint->advertiseAddress
change nodeRegistration->name
change imageRepository if needed
change kubernetesVersion if needed

add podSubnet: "192.168.0.0/16" after serviceSubnet, don't conflict with virtual machine host network cidr

kubeadm init --config=kubeadm.yml

##获取join命令
kubeadm token create --print-join-command

##slave加入
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

#bookmark
https://kubernetes.io/docs/tutorials/stateless-application/guestbook/

#network block
ufw status
ufw disable
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -F
route add default gw IP Address Adapter