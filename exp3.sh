ipaddr=`/sbin/ifconfig | grep 'inet' | grep -v '127.0.0.1' | cut -d : -f3 | awk '{print $1}'`
echo $ipaddr
