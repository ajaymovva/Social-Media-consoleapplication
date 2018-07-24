# define _CRT_SECURE_NO_WARNINGS
# include "structures.h"
#pragma once
/*
user_count
user_end
post_count
post_end

*/




void complete_remove(FILE *fp, char *username,int usercount,int user_end)
{

	user_info user;
	for (int iter = 0; iter < usercount; iter++)
	{

		int oldoffset = ftell(fp);
		fread(&user, sizeof(user_info), 1, fp);
		if (!strcmp(user.username, username))
		{
			fseek(fp, user_end-USER_BLOCK_SIZE, SEEK_SET);
			fread(&user, sizeof(user_info), 1, fp);
			fseek(fp, oldoffset, SEEK_SET);
			fwrite(&user, sizeof(user_info), 1, fp);
			fflush(fp);
			break;
		}
	}
}




void remove_user(FILE *fp)
{

	int usercount,user_end;
	char *username = (char *)malloc(sizeof(char) * 50);
	printf("enter username:\t");
	scanf("%s", username);
	fseek(fp, 0, SEEK_SET);
	metadata data;
	fread(&data, sizeof(metadata), 1, fp);
	user_end = data.next_insert_position;
	usercount = data.user_count;
	complete_remove(fp, username,usercount,user_end);
	data.user_count = usercount - 1;;
	data.next_insert_position = user_end - USER_BLOCK_SIZE;
	fseek(fp, 0, SEEK_SET);
	fwrite(&data, sizeof(metadata), 1, fp);
	fflush(fp);
}



int get_offset_for_name(FILE *fp,char *username)
{
	metadata data;
	fseek(fp, 0, SEEK_SET);
	fread(&data, sizeof(metadata), 1, fp);
	user_info temp;
	int usercount = data.user_count;
	for (int iter = 0; iter < usercount; iter++)
	{
		fread(&temp, sizeof(user_info), 1, fp);
		if (!strcmp(username, temp.username))
		{
			int x = ftell(fp);
			return (ftell(fp) - USER_BLOCK_SIZE);
		}
	}
	return -1;
}




void update_user(FILE *fp)
{
	int option;
	user_info userdata;
	char *user_name = (char *)malloc(sizeof(char) * 20);
	printf("enter username");
	scanf("%s", user_name);
	printf("enter [1]->name and [2]-> email ");
	scanf("%d", &option);
	fseek(fp, get_offset_for_name(fp,user_name), SEEK_SET);
	fread(&userdata, sizeof(user_info), 1, fp);
	if (option == 1)
	{
		printf("enter new_name:\t");
		scanf("%s", userdata.username);
	}
	else
	{
		printf("enter new_email:\t");
		scanf("%s", userdata.email);
	}
	fseek(fp, ftell(fp) - USER_BLOCK_SIZE, SEEK_SET);
	fwrite(&userdata, sizeof(user_info), 1, fp);
	fflush(fp);
}




void get_user(FILE *fp)
{
	char *user_name = (char *)malloc(sizeof(char) * 30);
	user_info userdata;
	printf("enter username:\t");
	scanf("%s", user_name);
	int offset = get_offset_for_name(fp, user_name);
	fseek(fp,offset , SEEK_SET);
	fread(&userdata, sizeof(user_info), 1, fp);
	if (offset!=-1)
	{
		printf("user details:\n");
		printf("  username is :%s \n", userdata.username);
		printf("  mailid: is :%s \n", userdata.email);
	}
}



void create_user(FILE *fp)
{


	int end, user_count=0;
	struct metadata data;
	fseek(fp, 0, SEEK_SET);
	fread(&data, sizeof(metadata), 1, fp);
	end = data.next_insert_position;
	user_count = data.user_count;
	user_info new_user;
	user_count++;
	new_user.userid = user_count;
	printf("enter username: ");
	scanf("%s", new_user.username);
	printf("enter emailid: ");
	scanf("%s", new_user.email);
	int x = ftell(fp);
	fseek(fp, end, SEEK_SET);
	fwrite(&new_user, sizeof(user_info), 1, fp);
	fflush(fp);

	int new_end = ftell(fp);
	data.next_insert_position = new_end;
	data.user_count = user_count;
	fseek(fp, 0, SEEK_SET);
	fwrite(&data, sizeof(metadata), 1, fp);
	fseek(fp, end, SEEK_SET);
	fread(&new_user, sizeof(user_info), 1, fp);
	fflush(fp);
	
}











void show_posts(char *username, FILE *fp)
{
	int offset = get_offset_for_name(fp,username);
	user_info user;
	post postitem;
	fseek(fp, offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	
	for (int iter = 0; iter < user.post_count; iter++)
	{
		int offset = user.posts[iter].post_offset;
		if (offset != -1)
		{
			fseek(fp, offset, SEEK_SET);
			fread(&postitem, sizeof(post), 1, fp);
			printf("postid:%d   ", postitem.postid);
			if (postitem.type == 2)
			{
				printf("%s\n", postitem.post_data);
			}
			else if (postitem.type == 1)
			{
				printf("filename :%s\n", postitem.filename);
			}
		}
	}
}






void get_all_messages(FILE *fp)
{
	char *username = (char *)malloc(sizeof(char) * 100);
	printf("enter username for messages:\t");
	scanf("%s", username);
	int offset = get_offset_for_name(fp, username);
	fseek(fp, offset, SEEK_SET);
	user_info user;
	fread(&user, sizeof(user), 1, fp);
	for (int iter = 0; iter < user.message_count; iter++)
	{
		if (user.messages[iter].receiver_user_id == user.userid)
		{
			printf("%s", user.messages[iter].message);
		}
	}
	printf("\n");
}




void message_user(FILE *fp)
{
	char *sender = (char *)malloc(sizeof(char) * 30);
	char *receiver = (char *)malloc(sizeof(char) * 30);
	char *message = (char *)malloc(sizeof(char) * 100);
	user_info user;
	printf("enter sender username:\t");
	scanf("%s", sender);
	printf("enter receiver username:\t");
	scanf("%s", receiver);
	printf("enter message:\t");
	scanf("%s", message);


	int sender_offset = get_offset_for_name(fp, sender);
	int receiver_offset = get_offset_for_name(fp, receiver);


	fseek(fp, receiver_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	int receiverid = user.userid;


	fseek(fp, sender_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	user.message_count += 1;
	int senderid = user.userid;
	user.messages[user.message_count - 1].sender_userid = user.userid;
	user.messages[user.message_count - 1].receiver_user_id = receiverid;
	strcpy(user.messages[user.message_count - 1].message, message);
	fseek(fp, sender_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);

	fseek(fp, receiver_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	user.message_count += 1;
	user.messages[user.message_count - 1].sender_userid = senderid;
	user.messages[user.message_count - 1].receiver_user_id = user.userid;
	strcpy(user.messages[user.message_count - 1].message, message);
	fseek(fp, receiver_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);
	fflush(fp);
}





void addcoment(FILE *fp, char *username)
{
	int postid;
	printf("enter post id to coment:\t");
	scanf("%d", &postid);
	int offset = get_offset_for_name(fp, username);
	user_info user;
	fread(&user, sizeof(user_info), 1, fp);
}



void print_coments(user_info user, int postid)
{
	for (int iter = 0; iter < user.posts[postid].coments_count; iter++)
	{
		printf("coment index: %d coment %s\n", iter, user.posts[postid].comments);
	}
}




void post_operations(FILE *fp, char *username,int postid)
{
	int offset = get_offset_for_name(fp, username);
	user_info user;
	fseek(fp, offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);
	int option;
	printf("enter [1] ->ADD  [2] REMOVE [3] VIEW for coments:\t");
	scanf("%d", &option);
	if (option == 1)
	{
		char *coment = (char *)malloc(sizeof(char) * 20);
		printf("enter coment:");
		scanf("%s", coment);
		int presentcoment=user.posts[postid].coments_count;
		strcpy(user.posts[postid].comments[presentcoment], coment);
		user.posts[postid].coments_count = user.posts[postid].coments_count + 1;
		fseek(fp, offset, SEEK_SET);
		fwrite(&user, sizeof(user_info), 1, fp);
		fflush(fp);
	}
	else if (option == 2)
	{
		print_coments(user,postid);
		printf("enter comentindex to remove:\t");
		int index;
		scanf("%d", &index);
		int count = user.posts[postid].coments_count-1;
		strcpy(user.posts[postid].comments[index], user.posts[postid].comments[count]);
		user.posts[postid].coments_count--;
		fseek(fp, offset, SEEK_SET);
		fwrite(&user, sizeof(user_info), 1, fp);
		fflush(fp);
	}
	else if (option == 3)
	{
		print_coments(user, postid);
	}

}


void comment(FILE *fp)
{

	int postid;
	char *username = (char *)malloc(sizeof(char) * 20);
	printf("enter username:\t");
	scanf("%s", username);
	show_posts(username, fp);
	printf("enter post id shown in above to do coment operations:\t");
	scanf("%d", &postid);
	post_operations(fp, username,postid);
}



void postNEW(char *name, FILE *fp)
{
	metadata md;
	user_info user;
	post new_post;
	int post_type;
	user_posts post_info;

	char *text, *image_path;

	int user_offset = get_offset_for_name(fp,name);
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	strcpy(new_post.username, user.username);
	new_post.user_id = user.userid;
	new_post.postid = user.post_count;

	printf("Enter post type: [1]-> File \t [2]-> Text \n");

	scanf("%d", &post_type);
	if (post_type == 1)
	{
		image_path = (char*)malloc(sizeof(char) * 50);
		printf("Enter path of the File:\n");
		fflush(stdin);
		gets(image_path);

	

		FILE  *fp1;
		fp1 = fopen(image_path, "rb");
		char *image;
		int size;
		fseek(fp1, 0L, SEEK_END);
		size = ftell(fp1);
		image = (char*)malloc(sizeof(char) * size);
		fseek(fp1, 0, SEEK_SET);
		fread(image, size, 1, fp1);

		strcpy(new_post.post_data, image);
		new_post.type = 1;
		strcpy(new_post.filename, image_path);
		fclose(fp1);

	}
	else if (post_type == 2)
	{
		fflush(stdin);
		text = (char*)malloc(sizeof(char) * 500);
		gets(text);
		strcpy(new_post.post_data, text);
		new_post.type = 2;
	}

	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(metadata), 1, fp);

	fseek(fp, md.next_post_position, SEEK_SET);
	fwrite(&new_post, sizeof(post), 1, fp);


	user_offset = get_offset_for_name(fp, name);

	user.posts[user.post_count].post_offset = md.next_post_position;
	user.post_count = user.post_count + 1;

	md.posts_count = md.posts_count + 1;
	md.next_post_position = md.next_post_position + POST_BLOCK_SIZE;


	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);

	fseek(fp, 0, SEEK_SET);
	fwrite(&md, sizeof(metadata), 1, fp);
	fflush(fp);
	show_posts(name, fp);
}




void update_post(char *name, FILE *fp)
{
	char *newtext = (char*)malloc(sizeof(char) * 50);
	int post_id, post_offset;
	post present_post;
	user_info user;
	int user_offset = get_offset_for_name(fp,name);

	show_posts(name, fp);

	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	printf("Enter postID from above you want to update:\n");
	scanf("%d", &post_id);
	post_offset = user.posts[post_id].post_offset;

	fseek(fp, post_offset, SEEK_SET);
	fread(&present_post, sizeof(post), 1, fp);


	if (present_post.type == 2)
	{
		fflush(stdin);
		printf("Enter new text you want to update:\n");
		gets(newtext);
		strcpy(present_post.post_data, newtext);
	}
	fseek(fp, post_offset, SEEK_SET);
	fwrite(&present_post, sizeof(post), 1, fp);
	fflush(fp);
	show_posts(name, fp);

}



void remove_post(char *name, FILE *fp)
{
	char *newtext;
	int post_id, post_offset;
	post present_post, last_post;
	user_info user, last_user;
	metadata md;
	fseek(fp, 0, SEEK_SET);
	fread(&md, sizeof(metadata), 1, fp);

	int user_offset = get_offset_for_name(fp,name);

	show_posts(name, fp);

	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	printf("Enter postID from above you want to rempve:\n");
	scanf("%d", &post_id);


	post_offset = user.posts[post_id].post_offset;
	user.posts[post_id].post_offset = -1;
	user.post_count = user.post_count - 1;


	fseek(fp, user_offset, SEEK_SET);
	fwrite(&user, sizeof(user_info), 1, fp);


	fseek(fp, post_offset, SEEK_SET);
	fread(&present_post, sizeof(post), 1, fp);

	fseek(fp, md.next_post_position - POST_BLOCK_SIZE, SEEK_SET);
	fread(&last_post, sizeof(post), 1, fp);

	fseek(fp, post_offset, SEEK_SET);
	fwrite(&last_post, sizeof(post), 1, fp);

	md.next_post_position = md.next_post_position - POST_BLOCK_SIZE;
	md.posts_count = md.posts_count - 1;

	fseek(fp, 0, SEEK_SET);
	fwrite(&md, sizeof(metadata), 1, fp);

	int last_user_offset = get_offset_for_name(fp,last_post.username);
	fseek(fp, last_user_offset, SEEK_SET);
	fread(&last_user, sizeof(user_info), 1, fp);
	last_user.posts[last_post.postid].post_offset = md.next_post_position - POST_BLOCK_SIZE;
	fseek(fp, last_user_offset, SEEK_SET);
	fwrite(&last_user, sizeof(user_info), 1, fp);
	fflush(fp);
	show_posts(name, fp);
}




void save_post(FILE *fp)
{
	int post_id, user_offset, post_offset;
	user_info user;
	post post;
	char *user_name = (char*)malloc(sizeof(char) * 20);
	printf("Enter user name :\n");
	fflush(stdin);
	gets(user_name);
	show_posts(user_name, fp);
	printf("Enter post ID you want to save:\n");
	scanf("%d", &post_id);

	user_offset = get_offset_for_name(fp,user_name);
	fseek(fp, user_offset, SEEK_SET);
	fread(&user, sizeof(user_info), 1, fp);

	post_offset = user.posts[post_id].post_offset;
	fseek(fp, post_offset, SEEK_SET);
	fread(&post, sizeof(post), 1, fp);

	char *data = (char*)malloc(sizeof(char) * 153536);

	strcpy(data, post.post_data);
	if (post.type == 1)
	{
		char *filePath = (char*)malloc(sizeof(char) * 30);
		printf("Enter file name you want to save:\n");
		fflush(stdin);
		gets(filePath);
		strcpy(data, post.post_data);
		FILE *fp1 = fopen(filePath, "wb");
		fseek(fp1, 0, SEEK_SET);
		fwrite(post.post_data, sizeof(post.post_data), 1, fp1);
		fclose(fp1);
	}
	fflush(fp);
}

