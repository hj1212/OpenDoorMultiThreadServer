#include "mydb.h"
#include "log.h"

CMyDB::CMyDB()
{   
    //��ʼ���������ݿ����
    connection = mysql_init(NULL);
    if(connection == NULL)
    {
        perror("mysql_init");
        exit(1);
    }
}
CMyDB::~CMyDB()
{   
    //�رճ�ʼ���������ݿ����
    if(connection != NULL)
    {
        mysql_close(connection);
    }
}
//��ʼ�����ݿ� ���ݿ�����
bool CMyDB::initDB(string server_host , string user, string password , string db_name )
{   
    //����mysql_real_connect����ʵ�����ݿ������
    connection_r = mysql_real_connect(connection , server_host.c_str() , user.c_str() , password.c_str() , db_name.c_str() , 0 , NULL , 0);
    if(connection_r == NULL)
    {
		printf("initDB connect error\n");
        perror("mysql_real_connect");
        exit(1);
    }
	else
	{
		printf("initDB connect success\n");
	}
    return true;
}

//ִ��SQL���
char*** CMyDB::executeSQL(string sql_str,int* numRows,  int* columnSizes)
{
	
	const char* c_s = sql_str.c_str();
	Fprint_LogString(c_s,LOG_OPENDOOR_FILE,"a+");

    // ��ѯ�����趨
	char*** r;
    if(mysql_query(connection, "set names utf8")){
        fprintf(stderr, "%d: %s\n",mysql_errno(connection), mysql_error(connection));
    }
    int t = mysql_query(connection,  sql_str.c_str());
    if(t){
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }else{   
        /*res = mysql_use_result(connection); //��ʼ�����еĽ��������
        if(res)
        {
            //mysql_field_count(connection)   ���������������ϵ������ѯ������
            for(int i = 0 ; i < mysql_field_count(connection) ; i++)
            {   
                //����һ��������ϵ���һ��
                row = mysql_fetch_row(res);   		
                if(row <= 0)
                {
                    break;
                }
                //mysql_num_fields(res)  �������ؽ�������ֶε���
                for(int r = 0 ; r  < mysql_num_fields(res) ; r++)
                {
                    printf("%s\t" , row[r]);    
                }
                printf("\n");
            }
        }
        mysql_free_result(res);//�ͷŽ����ʹ�õ��ڴ�*/
		
		//----------------------------------------------
		res=mysql_store_result(connection);
		if(res) //ע�⣺һ��Ҫ�жϣ��������޸ı��������Ϊ�޸Ĳ�û�л�ȡ���
		{
			int f1,f2,num_row, num_col;
			num_row = mysql_num_rows(res); /* Get the no. of row */
			num_col = mysql_num_fields(res); /* Get the no. of column */
			//printf("num_row = %d,num_col = %d\n",num_row,num_col);  //num_col ָ����Ԫ�أ�һ��С���񣩸����������ַ�����
			r = (char***)malloc(sizeof(char**) * num_row);
			*numRows = num_row;
			*columnSizes = num_col;
			for (f1 = 0; f1 < num_row; f1++) {
				row = mysql_fetch_row(res); /* Fetch one by one */
				r[f1] = (char**)malloc(sizeof(char*) * num_col);
				for (f2 = 0; f2 < num_col; f2++) {
					printf("%s\t",  row[f2]);
					r[f1][f2] = (char*)malloc(sizeof(char) * (strlen(row[f2])+1));
					strcpy(r[f1][f2],row[f2]);
					//printf("%s\t",  r[f1][f2]); //��仰��printf("%s\t",  row[f2]);һ����
				}
				printf("\n");
			}	
		}
		mysql_free_result(res);
		//mysql_close(sock);
    }
    return r;
}
//��Ĵ���
bool CMyDB::create_table(string table_str_sql)
{
    int t = mysql_query(connection , table_str_sql.c_str());
    if(t)
    {
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }
    return true;
}