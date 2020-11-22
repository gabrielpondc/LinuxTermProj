#include <linux/rbtree.h>
#include <linux/rbtree_latch.h>
#include <linux/rbtree_augmented.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#define CHECK_INSERT 1    // The detection switch of "insert" action (0, close; 1, open)
#define LENGTH(a) ( (sizeof(a)) / (sizeof(a[0])) )
#define N 10
struct rb_root mytree = RB_ROOT;
typedef int Type;
 
struct my_node {
    struct rb_node rb_node;    // rbtree node
    Type key;                // insert key
};
 

/*
 * Insert the key into the red-black tree. Insert successful, return 0; Failure returns -1.
 */
int my_insert(struct rb_root *root, Type key)
{
    struct my_node *mynode; // creat node
    struct rb_node **tmp = &(root->rb_node), *parent = NULL;
 
    /* Figure out where to put new node */
    while (*tmp)
    {
        struct my_node *my = container_of(*tmp, struct my_node, rb_node);
 
        parent = *tmp;
        if (key < my->key)
            tmp = &((*tmp)->rb_left);
        else if (key > my->key)
            tmp = &((*tmp)->rb_right);
        else
            return -1;
    }
 
    // Returns if the new node fails.
    if ((mynode=kmalloc(sizeof(struct my_node),GFP_KERNEL)) == NULL)
        return -1; 
    mynode->key = key;
    mynode->rb_node.__rb_parent_color = 0;
    mynode->rb_node.rb_left = NULL;
    mynode->rb_node.rb_right = NULL;
 
    /* Add new node and rebalance tree. */
    rb_link_node(&mynode->rb_node, parent, tmp);
    rb_insert_color(&mynode->rb_node, root);
 
    return 0;
}
/*
(gdb) p **tmp
Cannot access memory at address 0x0
(gdb) p *tmp
$1 = (struct rb_node *) 0x0
(gdb) p tmp
$2 = (struct rb_node **) 0x7fffffffe470
(gdb) n
64          mynode->key = key;
(gdb) n
65          mynode->rb_node.rb_parent_color = 0;
(gdb) n
66          mynode->rb_node.rb_left = NULL;
(gdb) n
67          mynode->rb_node.rb_right = NULL;
(gdb) n
70          rb_link_node(&mynode->rb_node, parent, tmp);
(gdb) n
71          rb_insert_color(&mynode->rb_node, root);
(gdb) p tmp
$3 = (struct rb_node **) 0x7fffffffe470
(gdb) p *tmp
$4 = (struct rb_node *) 0x604010
(gdb) p **tmp
$5 = {rb_parent_color = 0, rb_right = 0x0, rb_left = 0x0}
(gdb) p &mynode->rb_node
$6 = (struct rb_node *) 0x604010
(gdb) p mynode->rb_node
$7 = {rb_parent_color = 0, rb_right = 0x0, rb_left = 0x0}
(gdb)
*/
 
/*
 * print rbtree
 */
static void print_rbtree(struct rb_node *tree, Type key, int direction)
{
    if(tree != NULL)
    {   
        if(direction==0)    // tree is root node
            printk("%2d(B) is root\n", key);
        else                // tree is branch node
            printk("%2d(%lu %s) is %2d's %6s child\n", key, tree->__rb_parent_color,rb_is_black(tree)?"B":"R",  key, direction==1?"right" : "left");
            //printk("%2d(%s) is %2d's %6s child\n", key, rb_is_black(tree)?"B":"R", key, direction==1?"right" : "left");
 
        if (tree->rb_left)
            print_rbtree(tree->rb_left, rb_entry(tree->rb_left, struct my_node, rb_node)->key, -1);
        if (tree->rb_right)
            print_rbtree(tree->rb_right,rb_entry(tree->rb_right, struct my_node, rb_node)->key,  1); 
    }   
}
 
void my_print(struct rb_root *root)
{
    if (root!=NULL && root->rb_node!=NULL)
        print_rbtree(root->rb_node, rb_entry(root->rb_node, struct my_node, rb_node)->key,  0); 
}


int simple_init(void)
{	
	static LIST_HEAD(head);
	int a[N]={};
    mytree.rb_node = NULL;
    int i, ilen = LENGTH(a);
    printk("== Origin Data: ");
    for(i=0; i<ilen; i++)
    	a[i]=i;
        printk("%d ", a[i]);
    printk("\n");
 
    for (i=0; i < ilen; i++) 
    {
        my_insert(&mytree, a[i]);
#if CHECK_INSERT
        printk("== Insert Node: %d\n", a[i]);
        printk("== Tree detail: \n");
        my_print(&mytree);
        printk("\n");
#endif
 
    }

	return 0;
		
}
void simple_exit(void)
{
    printk(KERN_INFO "Removing %d RBtree test Modules\n",1);
}
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Team 9");
MODULE_AUTHOR("Linux team 9");
