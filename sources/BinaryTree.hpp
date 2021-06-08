#pragma once

#include <iostream>
#include <ostream>
#include <vector>



using namespace std;
namespace ariel {

    template<typename T>
    class BinaryTree {

    private:
        enum class Order {Preorder, Inorder, Postorder};
        struct Node {
            
            T data;
            Node *left;
            Node *right;

            
            Node(const T &data) : data(data), left(nullptr), right(nullptr) {}

            /**
             * copy constructor.
             */
            Node(const Node &node) : data(node.data){
                
                if (node.right != nullptr){
                    right = new Node(*(node.right));
                }
                if (node.left != nullptr){
                    left = new Node(*(node.left));
                }
            }

            /**
             * operator = 
             * nassary copy constructor
             */
            Node &operator=(Node &&node) noexcept{
                *this = std::move(node);
                return *this;
            }

            /**
             * move constructor.
             */
            Node(Node &&node) noexcept{
                *this = std::move(node);
            }

            /**
             * operator = 
             * nessary to move constructor.
             */
            Node &operator=(const Node &node){
                if (this != &node){
                    this = new Node(node);
                }
                return *this;
            }

            ~Node() {
                delete left;
                delete right;
            }
        }; 
        Node *root;
        uint size;

        /**
         * Return node by its data.
         * if the data isnt in the tree rhan it return nullptr.
         * @param data - what we want to find.
         * @param root - the root of the tree when it recusive then another node.
         */
        Node *find_node(T data, Node *root){
            if (root == nullptr){
                return root;
            }
            else if(root->data == data)
            {
                return root;
            }
            Node *left_side = find_node(data, root->left);
            Node *right_side = find_node(data, root->right);
            if(left_side == nullptr){
                return right_side;
            }
                return left_side;
        }


    public:
        BinaryTree() : root(nullptr), size(0){}
        ~BinaryTree() {  }

        /**
         * copy constructor.
         */
        BinaryTree(const BinaryTree &tree){
			if (tree.root != nullptr){
				root = new Node(tree.root->data);
                size = tree.size;
				deep_copy(root, tree.root);
			}
		}

        /**
         * operator = 
         * necessary to copy constructor.
         */
        BinaryTree &operator=(const BinaryTree<T> &tree){
			if (this == &tree){
				return *this;
			}else if (root != nullptr){
				delete root;
            }
			if (tree.root != nullptr){
				root = new Node{tree.root->data};
				deep_copy(root, tree.root);
			}
			return *this;
		}

         /**
         * move constructor.
         */
        BinaryTree(BinaryTree &&other) noexcept : size(other.size){
            *this = std::move(other);
        }

        /**
         * operator = 
         * necessary to move constructor.
         */
        BinaryTree &operator=(BinaryTree &&other) noexcept{
            *this = std::move(other);
            return *this;
        }

        /**
         * deep copy of the tree.
         * do it in recursive way.
         * @param my_root - the root of the tree.
         * @param other_root - the root of the other tree.
         */
          void deep_copy(Node *ptr1, const Node *ptr2){
			if (ptr2->right != nullptr){
				ptr1->right = new Node(ptr2->right->data);
				deep_copy(ptr1->right, ptr2->right);
			}

            if (ptr2->left != nullptr){
				ptr1->left = new Node(ptr2->left->data);
				deep_copy(ptr1->left, ptr2->left);
			}
		}


    
        /**
         * Add root on the tree.
         * If root already exist replace root's data.
         * @param data - the data of the root.
         */
        BinaryTree &add_root(T data){
            if(root == nullptr){
                size++;
                root = new Node(data);
            }
            else{
                root->data = data;
            }
            return *this;
        }

        /**
         * Add left child to giving node.
         * If left child exist to the giving node, replace its data.
         * @param exist_data - the data that already exists in the tree.
         * @param new_data - the new data to be added as his left child.
         */
        BinaryTree &add_left(T giving_data, T new_data) {
            Node *node = BinaryTree<T>::find_node(giving_data, root);
            //if the given data does not exist:
            if(node == nullptr){
                throw std::invalid_argument("The given data is not in the tree.\n");
            }
            //if the given data exist & dont have left child:
            if(node->left == nullptr){
                size++;
                node->left = new Node{new_data};
                
            }
            //if the given data exist & has left child:
            else{
                node->left->data = new_data;
            }
            return *this;
        }

        /**
         * Add right child to the giving node.
         * If right child exist to the giving node, replace its data.
         * @param exist_data - the data that already exists in the tree.
         * @param new_data - the new data to be added as his right child.
         */
        BinaryTree &add_right(T giving_data, T new_data) {
            Node *node = BinaryTree<T>::find_node(giving_data, root);
            //if the given data does not exist:
            if(node == nullptr){
                throw invalid_argument("no node with the given data\n");
            }
            //if the given data exist & dont have right child:
            if(node->right == nullptr){
                size++;
                node->right = new Node(new_data);
            }
            //if the given data exist & has right child:
            else{
                node->right->data = new_data;
            }
            return *this;
        }

        
        
        struct Iterator {

            private:
                Node *node_pointer;
                int index;
                Order order;//to know which order to usu(preorder, inorder, postorder).
                vector<Node *> container;
                void preorder(Node *ptr)
                {
                    if(!ptr) return;
                    container.insert(ptr);
                    preorder(ptr->left);
                    preorder(ptr->right);
                }
                void inorder(Node *ptr)
                {
                    if(!ptr) return;
                    preorder(ptr->left);
                    container.insert(ptr);
                    preorder(ptr->right);
                }
                void postorder(Node *ptr)
                {
                    if(!ptr) return;
                    preorder(ptr->left);
                    preorder(ptr->right);
                    container.insert(ptr);
                }

            public:

                /**
                 * construct the iterator by given order (preorder, inorder, postorder).
                 * @param node - the first node.
                 * @param order - preorder, inorder, postorder.
                 */
                Iterator(Node *node = nullptr ,Order order= Order::Inorder) :node_pointer(node), order(order) index(0){
                    if(node_pointer!=nullptr){
                        switch (order)
                        {
                        case Order::Preorder:
                            preorder(root);
                            break;
                        case Order::Inorder:
                            inorder(root);
                            break;
                        case Order::Postorder:
                            postorder(root);
                    }
                }

                ~Iterator(){}

                /**
                 * copy constructor.
                 */
                Iterator(const Iterator &iterator) : node_list(iterator.node_list), node_stack(iterator.node_stack), order(iterator.order){
                    node_pointer = iterator.node_pointer;
                }

                /**
                 * operator=
                 * nessary for copy constructor.
                 */
                Iterator &operator=(const Iterator &iterator){
                    if (this != &iterator){
                        node_pointer = iterator.node_pointer;
                        order = iterator.order;
                        container = iterator.container;
                    }
                    return *this;
                }

                /**
                 * move constructor.
                 */
                Iterator(Iterator &&iterator) noexcept: node_list(iterator.node_list), order(iterator.order){
                        *this = std::move(iterator);
                }

                /**
                 * operator= 
                 * nessary to move constructor.
                 */
                Iterator &operator=(Iterator &&iterator) noexcept{
                    node_pointer = std::move(iterator.node_pointer);
                    order = std::move(iterator.order);
                    container = std::move(iterator.container);
                    return *this;
                }

                T &operator*() const {
                    return node_pointer->data;
                }
                
                T *operator->() const {
                    return &(node_pointer->data);
                }

                /**
                 * operator ++i
                 */
                Iterator &operator++(){
                    index++;
                    node_pointer=container[index];
                    return *this;
                }

                /**
                 * operator i++
                 */
                Iterator operator++(int){
                    Iterator tmp= *this;
                    index++;
                    node_pointer=container[index];
                    return tmp;
		        }       

                /**
                 * operator ==
                 */
                bool operator==(const Iterator &iterator)const{
                    return node_pointer == iterator.node_pointer;
                }

                /**
                 * operator !=
                 */
                bool operator!=(const Iterator &iterator)const{
                    return node_pointer != iterator.node_pointer;
                }
            };
        /**
         * Return Iterator orderd by preorder.
         */
        Iterator begin_preorder(){
            return Iterator{root, Order::Preorder};
        }
        /**
         * Return an empty Iterator.
         */
        Iterator end_preorder(){
            return Iterator{nullptr, Order::Preorder};
        }
        /**
         * Return Iterator orderd by inorder.
         */
        Iterator begin_inorder(){
             return Iterator{root, Order::Inorder};
        }
        /**
         * Return an empty Iterator.
         */
        Iterator end_inorder(){
             return Iterator{nullptr, Order::Inorder};
        }
        /**
         * Return Iterator orderd by postorder.
         */
        Iterator begin_postorder(){
             return Iterator{root, Order::Postorder};
        }
        /**
         * Return an empty Iterator.
         */
        Iterator end_postorder(){
            return Iterator{nullptr, Order::Postorder};
        }
        /**
         * Return Iterator orderd by inorder.
         */
         Iterator begin(){
            return Iterator{root, Order::Inorder};
        }
        /**
         * Return an empty Iterator.
         */
        Iterator end(){
            return Iterator{nullptr,Order::Inorder};
        }


            //NOT WORK
            friend ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree){
            // Iterator iter = begin_inorder();
            // for (int i = 0; i < tree.size; i++)
            // {
            //     os<<itr++;
            // }
            // os<<endl;
            return os;
        } 
    };

}
