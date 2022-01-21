#搭建Kubernetes集群

##清理
kubeadm reset
rm -rf .kube /etc/kubernetes
service kubelet stop

##kubeadm命令行
kubeadm init --pod-network-cidr 172.16.0.0/16
kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml

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
ifconfig cni0 down
ifconfig flannel.1 down
ifconfig docker0 down
service docker start