# include "user_command_operations.h"



void post_information(FILE *fp)
{
	char *username = (char *)malloc(sizeof(char) * 20);
	int option;
	while (1)
	{
		printf("enter username:");
		scanf("%s", username);
		printf("\n[1]-> newpost \n[2]->updatepost \n[3]-> removepost \n[4]->savepost \n[5]-> comment \n[6]->exit \n");
		scanf("%d", &option);
		if (option == 6)
		{break;}
		switch (option)
		{
		case 1:
			postNEW(username, fp);
			break;
		case 2:
			update_post(username, fp);
			break;
		case 3:
			remove_post(username, fp);
			break;
		case 4:
			save_post(fp);
			break;
		case 5:
			comment(fp);
			break;
		}
	}
}



void user_information(FILE *fp)
{

	while (1)
	{
		int option;
		printf("[1]-> createuser  \n[2]->getuser \n[3]->update user \n[4]-> removeuser  \n[5] ->exit \n");
		fflush(stdin);
		scanf("%d", &option);
		if (option == 5){ break; }
		switch (option)
		{
		case 1:
			create_user(fp);
			break;
		case 2:
			get_user(fp);
			break;
		case 3:
			update_user(fp);
			break;
		case 4:
			remove_user(fp);
			break;
		default:
			break;
		}
	}
}



void message_info(FILE *fp)
{
	while (1)
	{
		printf("enter [1]->All messages \n [2]-> usermessages \n [3]-> exit \n");
		int option;
		fflush(stdin);
		scanf("%d", &option);
		if (option == 3)
		{
			break;
		}
		switch (option)
		{
		case 1:
			get_all_messages(fp);
			break;
		case 2:
			message_user(fp);
			break;
		default:
			break;
		}
	}
}





void All_menus()
{
	FILE *fp;
	fp = fopen("user_file_system.bin", "wb");
	fclose(fp);
	fp = fopen("user_file_system.bin", "rb+");
	struct metadata data;
	data.user_count = 0; data.posts_count = 0; data.next_insert_position = 16; data.next_post_position = 112900;
	fwrite(&data, sizeof(metadata), 1, fp);
	fflush(fp);
	char *username = (char *)malloc(sizeof(char) * 30);
	while (1)
	{
		int option;
		printf("[1] to userinfo \n[2] post info\n[3] message\n [4]->Exit\n");
		fflush(stdin);
		scanf("%d", &option);
		if (option == 4){ break; }
		switch (option)
		{
		case 1:
			user_information(fp);
			break;
		case 2:
			post_information(fp);
			break;
		case 3:
			message_info(fp);
			break;
		default:
			break;
		}
	}
	

	fclose(fp);
}


