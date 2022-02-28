export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0.0
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd \
-drive format=raw,file=fat:rw:root  -serial stdio -m 2G 
#-monitor stdio
#-serial stdio