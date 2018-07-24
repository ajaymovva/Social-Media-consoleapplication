# define USER_START_OFFSET 16
# define POST_START_OFFSET 112900
# define POST_BLOCK_SIZE 153600
# define USER_BLOCK_SIZE 2264


# include<stdio.h>
# include<stdlib.h>
# include<string.h>

int post_start_offset;
int post_end_offset;
int user_start_offset;


struct metadata
{
	int user_count;
	int next_insert_position;
	int posts_count;
	int next_post_position;
};


struct post
{
	int user_id;
	int postid;
	int type;
	char filename[32];
	char username[20];
	char post_data[153536];
};


struct user_posts
{
	int post_offset;
	int likes=0;
	int coments_count=0;
	char comments[5][20];
};



struct message_info
{
	int sender_userid;
	int receiver_user_id;
	char message[100];
};



struct user_info
{
	int userid;
	char username[20];
	char email[30];
	int post_count=0;
	int message_count=0;
	struct message_info messages[10];
	struct user_posts posts[10];
};



