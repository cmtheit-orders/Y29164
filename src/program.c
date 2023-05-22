/* A simplified community detection algorithm:
 *
 * Skeleton code written by Jianzhong Qi, May 2023
 * Edited by: [Add your name and student ID here for automatic recognition]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */


#define MAX_USER_NUM 50

typedef struct {
    int  number;
	char join_time[5];
    char hashtags[10][21];
    size_t hash_tag_num;
    int friendship[MAX_USER_NUM];
} user_t;

user_t users[MAX_USER_NUM] = {0};
size_t user_num = 0;

double user_soc(user_t * u1, user_t * u2) {
    if (u1->friendship[u2->number]) {
        int inter = 0;
        int uni = 0;
        for (int i = 0; i < user_num; ++i) {
            if (u1->friendship[i] && u2->friendship[i]) {
                inter++;
            }
            if (u1->friendship[i] || u2->friendship[i]) {
                uni++;
            }
        }
        return inter * 1.0 / uni;
    } else {
        return 0.0;
    }
}

void user_strength_print(double strength) {
    printf("%4.2lf", strength);
}

typedef char * data_t;							  /* to be modified for Stage 4 */

/* linked list type definitions below, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
*/
typedef struct node node_t;

struct node {
	data_t data;
	node_t *next;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;

/****************************************************************/

/* function prototypes */
list_t *make_empty_list(void);
void free_list(list_t *list);
void print_list(list_t *list);
list_t *insert_unique_in_order(list_t *list, data_t value);

void print_stage_header(int stage_num);

void stage_one();
void stage_two();
void stage_three();
void stage_four();

/* add your own function prototypes here */

/****************************************************************/

/* main function controls all the action; modify if needed */
int
main(int argc, char *argv[]) {
	/* add variables to hold the input data */

	/* stage 1: read user profiles */
	stage_one();

	/* stage 2: compute the strength of connection between u0 and u1 */
	stage_two();

	/* stage 3: compute the strength of connection for all user pairs */
	stage_three();

	/* stage 4: detect communities and topics of interest */
	stage_four();

	/* all done; take some rest */
	return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type
   and parameter list
*/

int stdpeek() {
    int p = getc(stdin);
    fseek(stdin, -1, SEEK_CUR);
    return p;
}

/* stage 1: read user profiles */
void
stage_one() {
	/* add code for stage 1 */
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);
    user_t * max_user_p = NULL;
    while (stdpeek() == 'u') {
        user_t * the_user = users + user_num;
        scanf("u%d %s", &the_user->number, the_user->join_time);
        size_t hash_tag_idx = 0;
        while (stdpeek() != '\n') {
            scanf(" #%s", the_user->hashtags[hash_tag_idx++]);
        }
        getchar();
        the_user->hash_tag_num = hash_tag_idx;
        if (!max_user_p || max_user_p->hash_tag_num < the_user->hash_tag_num) {
            max_user_p = the_user;
        }
        user_num++;
    }
    printf("Number of users: %lu\n", user_num);
    printf("u%d has the largest number of hashtags:\n", max_user_p->number);
    for (size_t i = 0; i < max_user_p->hash_tag_num; ++i) {
        printf("#%s ", max_user_p->hashtags[i]);
    }
    printf("\n");
	printf("\n");
}

/* stage 2: compute the strength of connection between u0 and u1 */
void
stage_two() {
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);
    for (int i = 0; i < user_num; ++i){
        for (int j = 0; j < user_num; ++j){
            scanf("%d", users[i].friendship + j);
        }
    }
    printf("Strength of connection between u0 and u1: ");
    user_strength_print(user_soc(users + 0, users + 1));
    printf("\n");
	printf("\n");
}

/* stage 3: compute the strength of connection for all user pairs */
void
stage_three() {
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);
    for (int i = 0; i < user_num; ++i) {
        for (int j = 0; j < user_num; ++j) {
            user_strength_print(user_soc(users + i, users + j));
            printf(" ");
        }
        printf("\n");
    }
	printf("\n");
}

/* stage 4: detect communities and topics of interest */
void stage_four() {
	/* add code for stage 4 */
	/* print stage header */
	print_stage_header(STAGE_NUM_FOUR);
    double ths;
    int thc;
    scanf("%lf%d", &ths, &thc);
    for (int i = 0; i < user_num; ++i) {
        int friendship = 0;
        for (int j = 0; j < user_num; ++j) {
            if (user_soc(users + i, users + j) > ths) {
                friendship++;
            }
        }
        if (friendship > thc) {
            list_t * hashtags = make_empty_list();
            for (int k = 0; k < users[i].hash_tag_num; ++k) {
                insert_unique_in_order(hashtags, users[i].hashtags[k]);
            }
            printf("Stage 4.1. Core user: u%d; close friends: ", i);
            for (int j = 0; j < user_num; ++j) {
                if (user_soc(users + i, users + j) > ths) {
                    printf("u%d ", j);
                    for (int k = 0; k < users[j].hash_tag_num; ++k) {
                        insert_unique_in_order(hashtags, users[j].hashtags[k]);
                    }
                }
            }
            printf("\n");
            printf("Stage 4.2. Hashtags:\n");
            print_list(hashtags);
            printf("\n");
            free_list(hashtags);
        }
    }
	printf("\n");
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* linked list implementation below, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
*/

/* create an empty list */
list_t
*make_empty_list(void) {
	list_t *list;

	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;

	return list;
}

/* free the memory allocated for a list (and its nodes) */
void
free_list(list_t *list) {
	node_t *curr, *prev;

	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}

	free(list);
}

/* insert a new data element into a linked list, keeping the
	data elements in the list unique and in alphabetical order
*/
list_t
*insert_unique_in_order(list_t *list, data_t value) {
	node_t *new;

	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
        node_t * crt = list->head;
        int cmp_result = strcmp(value, crt->data);
        if (cmp_result < 0) {
            new->next = list->head;
            list->head = new;
        } else {
            while (crt->next) {
                cmp_result = strcmp(value, crt->next->data);
                if (cmp_result <= 0) {
                    break;
                } else {
                    crt = crt->next;
                }
            }
            if (cmp_result != 0) {
                new->next = crt->next;
                crt->next = new;
                if (!crt->next) {
                    list->foot = new;
                }
            }
        }
	}
	return list;
}

/* print the data contents of a list */
void
print_list(list_t *list) {
	/* add code to print list */
    if (list) {
        node_t * head = list->head;
        while (head) {
            printf("#%s ", head->data);
            head = head->next;
        }
    }
}
/****************************************************************/
/*
	Write your time complexity analysis below for Stage 4.2,
	assuming U users, C core users, H hashtags per user,
	and a maximum length of T characters per hashtag:

    in source code we can conclude: O(stage_four) = O(U * (U * O(user_soc) + U * (O(user_soc) + H * O(intersection))))
    while `O(user_soc)` is equal to: O(U)
    and `O(intersection)` is equal to: O(H * T)
    so
        O(stage_four) = O(U ^ 3 + U * H * T)
*/
