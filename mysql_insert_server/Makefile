ALL: contacts mysql_insert_server

contacts: contacts.c
	gcc contacts.c -o contacts

mysql_insert_server: mysql_insert_server.c
	gcc -O mysql_insert_server.c `mysql_config --cflags --libs` -o contacts_server

clean:
	rm contacts contacts_server
