To run the program, some libraries need to be installed:
Way 1:
zmq:
	dnf install zeromq-devel
curl:
	
	
	
	
Way 2:

sudo vi /etc/ld.so.conf.d/myproject.conf
/home/gyd/myproject/test_CentOS7/lib
/home/gyd/myproject/test_CentOS7/lib/libevent-2.1.11
/home/gyd/myproject/test_CentOS7/pcap-1.9.1/lib
/home/gyd/myproject/test_CentOS7/lib/openssl
/home/gyd/myproject/test_CentOS7/zeromq-4.2.5/lib
/usr/local/boost/lib/
/home/gyd/myproject/test_CentOS7/include/amqpcpp/lib
/home/gyd/myproject/test_CentOS7/mysqlclient/lib
/home/gyd/myproject/test_CentOS7/mysql-connector-c++/lib
/home/gyd/myproject/test_CentOS7/cJSON/lib
/home/gyd/myproject/test_CentOS7/lib/netfilter

sudo ldconfig

Way 3:

export project_path=/home/gyd/myproject/test_CentOS7
export LD_LIBRARY_PATH=$project_path/lib:\
$project_path/lib/libevent-2.1.11:\
$project_path/pcap-1.9.1/lib:\
$project_path/lib/openssl:\
$project_path/zeromq-4.2.5/lib:\
/usr/local/boost/lib/:\
$project_path/include/amqpcpp/lib:\
$project_path/mysqlclient/lib:\
$project_path/mysql-connector-c++/lib:\
$project_path/cJSON/lib:\
$project_path/lib/netfilter:$LD_LIBRARY_PATH