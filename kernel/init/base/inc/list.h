#ifndef _LIST_H_
#define _LIST_H_
struct list_head {
	struct list_head *prev,*next;
};

#define INIT_LIST_HEAD(head) do {\
								(head)->prev=(head);(head)->next=(head);\
								} while(0)

int is_list_empty(struct list_head *head);

void list_add_head(struct list_head *new_node, struct list_head *head);

void list_add_tail(struct list_head *new_node, struct list_head *head);

void list_del(struct list_head *entry);

#define offsetof(type,member) (int)(&(((type *)0)->member))
#define list_entry(list_ptr, type, member) ((type *)((char *)(list_ptr)-offsetof(type,member)))
#endif
