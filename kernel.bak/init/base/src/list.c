#include "list.h"
int is_list_empty(struct list_head *head)
{
	return head->next == head;
}

void list_add_head(struct list_head *new_node, struct list_head *head)
{
	new_node->next = head;
	new_node->prev = head->prev;
	head->prev = new_node;
}

void list_add_tail(struct list_head *new_node, struct list_head *head)
{
	//首先找到最后一个元素
	struct list_head *last_node = head;
	while(last_node->next != head)
	{
		last_node = last_node->next;
	}
	//插入
	new_node->prev = last_node;
	new_node->next = last_node->next;
	last_node->next = new_node;
	head->prev = new_node;
}

void list_del(struct list_head *entry)
{
	// 链表空 了
	if(is_list_empty(entry))
	{
		return;
	}
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
	INIT_LIST_HEAD(entry);
}
