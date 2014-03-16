/*
* The GetPref application
* It's example how to search and to get the data which based on the phone prefix.
* Used by me for small billing queries for getting cost of a destinations.
* 
* (C) 2014 by Andrey I. Bekhterev (info@behterev.su)
* Released under the GPL
*
*/


#include <mysql/my_global.h>
#include <mysql.h>
#include <unistd.h> //GETOPT

#ifdef DEBUG
#define DEBUG_TRACE(x) printf("DEBUG: %s\n", (x))
#else
#define DEBUG_TRACE(x)
#endif

#define MY_HOST "localhost"
#define MY_LIGIN "iptel"
#define MY_PASS "pass4iptel"
#define MY_DB "iptel"

void finish_with_error(MYSQL *con)
{
	DEBUG_TRACE("MySQL finish_with_error");
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(EXIT_FAILURE);        
}

void print_usage()
{
	fprintf(stderr, "Usage: getpref -n <phone_number>\n\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	int opt;
	char debug[255];
	char* number;

	if (argc<2){
		DEBUG_TRACE("No argv parameters");
		print_usage();
	}

	while ((opt = getopt(argc, argv, "n:")) != -1) {
		switch (opt) {
			case 'n':
				number = optarg;
				break;
			default:
				print_usage();
			}
	}



	// +DEBUG
	snprintf(debug, sizeof(debug), "MySQL client version: %s",
		       	mysql_get_client_info());
	DEBUG_TRACE(debug);
	// -DEBUG

	DEBUG_TRACE("Init MySQL library");

	MYSQL *con = mysql_init(NULL);

	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	DEBUG_TRACE("Trying connection to MySQL server");

	if (mysql_real_connect(con, MY_HOST, MY_LIGIN, MY_PASS, 
				    MY_DB, 0, NULL, 0) == NULL) 
	{
		finish_with_error(con);
	}

	char query[255];
	snprintf(query,sizeof(query),"SELECT * FROM mtt WHERE '%s' LIKE " 
					"CONCAT(prefix,'%') ORDER BY prefix "
				       	"DESC LIMIT 1",number);

	// + DEBUG
	snprintf(debug, sizeof(debug), "Mysql query: %s", query);
	DEBUG_TRACE(debug);
	// - DEBUG

	if (mysql_query(con, query))
	{
		finish_with_error(con);
	}

	DEBUG_TRACE("Store results of query");

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) 
	{
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);
	// + DEBUG
	snprintf(debug, sizeof(debug), "Number of fields: %d",num_fields);
	DEBUG_TRACE(debug);
	// - DEBUG

	int num_rows = mysql_num_rows(result);
	// + DEBUG
	snprintf(debug, sizeof(debug), "Number of rows: %d",num_rows);
	DEBUG_TRACE(debug);
	// - DEBUG

	if(num_rows>0){

		MYSQL_ROW row;
	
		while ((row = mysql_fetch_row(result)))
		{
			float cost = atoi(row[1]);
			cost /= 100;
			printf("Result: %s\t%f\t%s\n",row[0],cost,row[2]);
		}
	}

	DEBUG_TRACE("Free MySQL result");

	mysql_free_result(result);

	DEBUG_TRACE("Close MySQL connection");

	mysql_close(con);

	exit(0);
}
