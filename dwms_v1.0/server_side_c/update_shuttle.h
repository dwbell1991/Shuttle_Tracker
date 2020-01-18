#ifndef UPDATE_SHUTTLE

#include </usr/include/mysql/my_global.h>
#include </usr/include/mysql/my_config.h>
#include <mysql.h>

#define MAX_BUFFER_SIZE 255

int startSQL(unsigned int id, char *utc, char *lat, char *lon, char *conn, char *satView);
int connect_mysql(MYSQL *con);
int update_mysql(MYSQL *con, unsigned int id, char *utc, char *lat, char *lon, char *conn, char *satView);
void disconnect_mysql(MYSQL *con);
void error_mysql(MYSQL *con);


int startSQL(unsigned int id, char *utc, char *lat, char *lon, char *conn, char *satView){
  //Allocate for MYSQL object  
  MYSQL *con = mysql_init(NULL);
  if (con == NULL){
      error_mysql(con);
      return -1;
  }
  if(connect_mysql(con) < 0){
	  error_mysql(con);
	  return -1;
  }
  if(update_mysql(con, id, utc, lat, lon, conn, satView) < 0){
	  disconnect_mysql(con);
	  error_mysql(con);
	  return -1;
  }
  disconnect_mysql(con);
  return 1;
}

/************************************************
 Name   : connect_mysql
 Params : con - MYSQL object
 Desc   : Sets up connection to MYSQL database
 ************************************************/
int connect_mysql(MYSQL *con){
	if(mysql_real_connect(con, "localhost", "dwbell_remote", "awsec21033", "shuttles", 0, NULL, 0) == NULL){
		error_mysql(con);
		return -1;
	}
	return 1;
}

/****Please note, this method is not done yet**********
  
  1)Pass the struct here
  2)Dissect the struct and assign it appropiate variables
  3)Use those appropiate variables to create a string
  4)The string gets passed as the update argument to MYSQL
  
  Note: Will need to check buffer size
        Will need a way to update shuttle code (bus number) 
                >Perhaps pass it with the struct from the PI
	May seperate methods one for string creation and one for updating
        Only update if were sure its a new coord (It should always be if PI is done right)
*/
int update_mysql(MYSQL *con, unsigned int id, char *utc, char *lat, char *lon, char *conn, char *satView){
  char buffer[MAX_BUFFER_SIZE]; /* Message size */
  sprintf(buffer, "UPDATE shuttle SET utcTime = %s, lat = %s, lng = %s, conn = %s, satView = %s WHERE id = %d", utc, lat, lon, conn, satView, id);
  if(mysql_query(con, buffer) < 0){
	  error_mysql(con);
	  disconnect_mysql(con);
	  return -1;
  }
  return 1;
}


/************************************************
 Name   : disconnect_mysql
 Params : con - MYSQL object
 Desc   : Disconnect previously established 
 	  connection.
 ************************************************/
void disconnect_mysql(MYSQL *con){
  mysql_close(con);
}

/************************************************
 Name   : error_mysql
 Params : con - MYSQL object
 Desc   : In case of an error...
          print > close conn > close program
 ************************************************/
void error_mysql(MYSQL *con){
  fprintf(stderr, "%s\n", mysql_error(con));
  disconnect_mysql(con);
  exit(1);
}

#endif
