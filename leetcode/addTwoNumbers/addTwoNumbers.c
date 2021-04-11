#include <stdio.h>
#include <stdlib.h>

struct ListNode {
	int val;
	struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2)
{
	int flag = 0;
	int i = 0, j = 0, max = 0;
	struct ListNode *p = l1, *q = l2;
	struct ListNode *target = NULL;

	while(p) {
		i ++;
		p = p->next;
	}

	while(q) {
		j ++;
		q = q->next;
	}

	if (i >= j) {
		max = i;
		p = l1;
		q = l2;
	} else {
		max = j;
		p = l2;
		q = l1;
	}

	target = (struct ListNode *)malloc((max + 1) * sizeof(struct ListNode));

    memset((void *)target, 0, (max + 1) * sizeof(struct ListNode));

	j = 0;

	while (q) {
		target[j].val = q->val + p->val + flag;
		flag = 0;
		if (target[j].val >= 10) {
			flag = target[j].val / 10;
			target[j].val %= 10;
		}
		j ++;
		q = q->next;
		p = p->next;
	}
	while (p) {
		target[j].val = p->val + flag;
		flag = 0;
		if (target[j].val >= 10) {
			flag = target[j].val / 10;
			target[j].val %= 10;
		}
		j ++;
		p = p->next;
	}

	if (flag > 0)
		target[j].val = flag;

	for(j = 0; j < max; j++) {
		target[j].next = &target[j+1];
		if (j == max -1 && target[max].val == 0)
			target[j].next = NULL;
	}
	return target;
}

int main()
{
	struct ListNode l1;
	struct ListNode l11;
	struct ListNode l12;
	struct ListNode l2;
	struct ListNode l21;
	struct ListNode l22;

	struct ListNode *p;

//	l1.val = 0;
//	l1.next = NULL;

	l1.val = 9;
	l1.next = NULL;

	l11.val = 9;
	l11.next = &l12;

	l12.val = 9;
	l12.next = NULL;

//	l2.val = 0;
//	l2.next = NULL;

	l2.val = 9;
	l2.next = &l21;

	l21.val = 9;
	l21.next = &l22;

	l22.val = 9;
	l22.next = NULL;

	p = &l1;
	while(p) {
		printf("l1 num:%d\n", p->val);
		p = p->next;
	}

	p = &l2;
	while(p) {
		printf("l2 num:%d\n", p->val);
		p = p->next;
	}

	p = addTwoNumbers(&l1, &l2);
	while (p) {
		printf("l3 num:%d\n", p->val);
		p = p->next;
	}

	return 0;
}
