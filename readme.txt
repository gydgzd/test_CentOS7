To run the program, some libraries need to be installed:
Way 1:
zmq:
	dnf install zeromq-devel
curl:
	
	
	
	
Way 2:

sudo vi /etc/ld.so.conf.d/myproject.conf
/home/gyd/myproject/test_CentOS7/thirdlib/curl/lib
/home/gyd/myproject/test_CentOS7/thirdlib/libevent-2.1.11/lib
/home/gyd/myproject/test_CentOS7/thirdlib/pcap-1.9.1/lib
/home/gyd/myproject/test_CentOS7/thirdlib/openssl-1.1.1d/lib
/home/gyd/myproject/test_CentOS7/thirdlib/zeromq-4.2.5/lib
/usr/local/boost/lib/
/home/gyd/myproject/test_CentOS7/thirdlib/amqpcpp/lib
/home/gyd/myproject/test_CentOS7/thirdlib/mysqlclient/lib
/home/gyd/myproject/test_CentOS7/thirdlib/mysql-connector-c++/lib
/home/gyd/myproject/test_CentOS7/thirdlib/cJSON/lib
/home/gyd/myproject/test_CentOS7/thirdlib/netfilter/libmnl/lib
/home/gyd/myproject/test_CentOS7/thirdlib/netfilter/libnetfilter_queue/lib
/home/gyd/myproject/test_CentOS7/thirdlib/netfilter/libnfnetlink/lib

sudo ldconfig

Way 3:

export project_path=/home/gyd/myproject/test_CentOS7
export LD_LIBRARY_PATH=$project_path/thirdlib/curl/lib:\
$project_path/thirdlib/libevent-2.1.11/lib:\
$project_path/thirdlib/pcap-1.9.1/lib:\
$project_path/thirdlib/openssl-1.1.1d/lib:\
$project_path/thirdlib/zeromq-4.2.5/lib:\
/usr/local/boost/lib/:\
$project_path/thirdlib/amqpcpp/lib:\
$project_path/thirdlib/mysqlclient/lib:\
$project_path/thirdlib/mysql-connector-c++/lib:\
$project_path/thirdlib/cJSON/lib:\
$project_path/thirdlib/netfilter/libmnl/lib:\
$project_path/thirdlib/netfilter/libnetfilter_queue/lib:\
$project_path/thirdlib/netfilter/libnfnetlink/lib:$LD_LIBRARY_PATH
