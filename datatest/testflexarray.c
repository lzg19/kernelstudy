#include <linux/flex_array.h>

struct testnode{
	struct hlist_node node;
	int a;
	int b;
};

struct testrcu{
	struct rcu_head *rcu;
	struct flex_array* buckets;
}

struct testrcu *g_rcu = NULL;


struct flex_array* init_array(){
	struct flex_array *buckets;
	buckets = flex_array_alloc(sizeof(struct hlist_head), 100, GRP_KERNEL|GFP_ZERO);
	if (!buckets){
		printk(KERN_INFO "flex array allocated failed\n");
		return NULL;
	}
	//in case flex_array_put gets blocked
	err = flex_array_prealloc(buckets, 0, 100, GFP_KERNEL|GFP_ZERO);
	if (err){
		flex_array_free(buckets);
		return NULL;
	}
	for (int i = 0; i < 100; i++){
		INIT_HLIST_HEAD((struct hlist_head*)flex_array_get(buckets,i));
	}
	return buckets;
}

void insert_node(){
	struct testnode *tn1;
	struct hlist_head *head;
	tn1 = kmalloc(sizeof(struct testnode), GFP_KERNEL|GFP_ZERO);
	if (!tn1){
		printk("alloc failed\n");
		return;
	}
	tn1->a = 100;
	tn1->b = 200;
	head = flex_array_get(g_rcu.buckets, 0);
	hlist_add_head_rcu(&tn1->node, head);  	
}

void traverse_nodes(){
	struct testnode *tn1;
	struct hlist_head *head;
	head = flex_array_get(g_rcu.buckets, 0);
	hlist_for_each_entry_rcu(tn1, head,node){
		printk("traverse information with %d, %d\n", tn1->a, tn1->b);
	}	
}

static void release_rcu(struct rcu_head *rcu){
	struct testrcu *tmprcu = container_of(rcu, struct testrcu, rcu);
	flex_array_free_parts(tmprcu->buckets);
	flex_array_free(tmprcu->buckets);
	kfree(tmprcu);
}

void destroy_all(){
	struct testnode *tn1;
	struct hlist_head *head, *n;
	head = flex_array_get(g_rcu.buckets, 0);
	hlist_for_each_entry_safe(tn1, n, head, node){
		kfree(tn1);
	}
	call_rcu(&g_rcu->rcu, release_rcu);	
}

void start_and_run(){
	g_rcu = kmalloc(sizeof(struct testrcu), GFP_KERNEL); 
	g_rcu->buckets = init_array();
	insert_nodes();
	destroy_all(); 
}
