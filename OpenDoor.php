<?php
	//����
	exec("gpio mode 29 out");/*mode 0�������߲������ţ�����ʹ��0-7*/
	usleep(100000);
	exec("gpio write 29 0");/*����led*/
	exec("gpio mode 29 in");/*mode 0�������߲������ţ�����ʹ��0-7*/
?>