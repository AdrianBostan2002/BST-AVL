#include <iostream>
#include <fstream>
#include <queue>

class AVL
{
private:
	class Node
	{
	public:
		int key;
		int height;
		Node* left;
		Node* right;
		Node* parent;
		Node(int key)
		{
			this->key = key;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
			height = 1;
		}
	};

	Node* root;

public:
	AVL()
	{
		root = nullptr;
	}

	~AVL()
	{
		clear_tree();
	}

	void rot_st(Node* y)
	{
		Node* x = y->right;
		if (x != nullptr)     //erroare pentru ca nu avem x->left daca x e nullptr
		{
			y->right = x->left;
		}
		else return;
		if (x->left != nullptr)
		{
			x->left->parent = y;
		}
		x->parent = y->parent;
		if (y->parent == nullptr)
		{
			root = x;
		}
		else
		{
			if (y == y->parent->left)
			{
				y->parent->left = x;
			}
			else
			{
				y->parent->right = x;
			}
		}
		x->left = y;
		y->parent = x;
		int height_left = 0, height_right = 0;
		getNodeHeight(y, height_left, height_right);
		y->height = std::max(height_left, height_right) + 1;
		height_left = 0, height_right = 0;
		getNodeHeight(x, height_left, height_right);
		x->height = std::max(height_left, height_right) + 1;
	}

	void rot_dr(Node* y)
	{
		Node* x = y->left;
		if (x != nullptr)     //erroare pentru ca nu avem x->right daca x e nullptr
		{
			y->left = x->right;
		}
		if (x->right != nullptr)
		{
			x->right->parent = y;
		}
		x->parent = y->parent;
		if (y->parent == nullptr)
		{
			root = x;
		}
		else
		{
			if (y == y->parent->right)
			{
				y->parent->right = x;
			}
			else
			{
				y->parent->left = x;
			}
		}
		x->right = y;
		y->parent = x;
		int height_left = 0, height_right = 0;
		getNodeHeight(y, height_left, height_right);
		y->height = std::max(height_left, height_right) + 1;
		height_left = 0, height_right = 0;
		getNodeHeight(x, height_left, height_right);
		x->height = std::max(height_left, height_right) + 1;
	}

	void insert(int key)
	{
		Node* nod_de_inserat = new Node(key);
		Node* x = root;
		Node* y = nullptr;
		while (x != nullptr)
		{
			y = x;
			if (nod_de_inserat->key < x->key)
			{
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		nod_de_inserat->parent = y;
		if (y == nullptr)
		{
			root = nod_de_inserat;
		}
		else
		{
			if (nod_de_inserat->key < y->key)
			{
				y->left = nod_de_inserat;
			}
			else
			{
				y->right = nod_de_inserat;
			}
			insert_repare(y);
		}

	}

	void insert_repare(Node* y)
	{
		while (y != nullptr)
		{
			int height_left = 0, height_right = 0;
			getNodeHeight(y, height_left, height_right);
			y->height = std::max(height_left, height_right) + 1;

			if (height_right - height_left > 1)
			{
				if (y->right->height == 1)
				{
					rot_st(y);
					break;
				}
				else
				{
					rot_dr(y->right);
					rot_st(y);
				}
			}
			if (height_right - height_left < -1)
			{
				if (y->left->height == -1)
				{
					rot_dr(y);
					break;
				}
				else
				{
					rot_st(y->left);
					rot_dr(y);
				}
			}
			if (height_right - height_left == 0)
			{
				break;
			}
			y = y->parent;
		}
	}

	void getNodeHeight(Node* y, int& height_left, int& height_right)
	{
		if (y->left != nullptr && y->left->height > height_left)
		{
			height_left = y->left->height;
		}
		if (y->right != nullptr && y->right->height > height_right)
		{
			height_right = y->right->height;
		}
	}

	void delete_key(int key)
	{
		Node* nod = find(key);
		if (nod != nullptr)
		{
			erase_node(nod);
		}
	}

	void delete_repare(Node* y, bool isRoot = false)
	{
		while (y != nullptr)
		{
			int height_left = 0, height_right = 0;
			getNodeHeight(y, height_left, height_right);
			y->height = std::max(height_left, height_right) + 1;

			if (height_right - height_left > 1)
			{
				if (height_right - height_left == -2)
				{
					rot_dr(y->right);
					rot_st(y);
				}
				else
				{
					rot_st(y);
					break;
				}
			}
			if (height_right - height_left < -1)
			{
				if (height_right - height_left == 2)
				{
					rot_st(y->left);
					rot_dr(y);
				}
				else
				{
					rot_dr(y);
					break;
				}
			}
			if (height_right - height_left == -1 || height_right - height_left == 1)
			{
				break;
			}
			y = y->parent;
		}
	}

	void erase_node(Node* z)
	{
		bool isRoot = false;
		Node* y;
		if (z->left == nullptr)
		{
			Transplant(z, z->right, isRoot);
			if (isRoot == true)
			{
				delete_repare(z, isRoot);
			}
			else delete_repare(z->parent, isRoot);
		}
		else
		{
			if (z->right == nullptr)
			{
				Transplant(z, z->left, isRoot);
				if (isRoot == true)
				{
					delete_repare(z, isRoot);
				}
				else delete_repare(z->parent, isRoot);
			}
			else
			{
				y = getSuccesor(z);
				if (y != z->right)
				{
					Transplant(y, y->right, isRoot);
					y->right = z->right;
					z->right->parent = y;
				}
				Transplant(z, y, isRoot);
				y->left = z->left;
				z->left->parent = y;
				if (isRoot == true)
				{
					delete_repare(y, isRoot);
				}
				else delete_repare(y->parent, isRoot);
			}
		}
	}

	void Transplant(Node* u, Node* v, bool& isRoot)
	{
		if (u->parent == nullptr)
		{
			root = v;
			isRoot = true;
		}
		else
		{
			if (u == u->parent->left)
			{
				u->parent->left = v;
			}
			else
			{
				u->parent->right = v;
			}
		}
		if (v != nullptr)
		{
			v->parent = u->parent;
		}
	}

	void construct(std::vector<int>vector)
	{
		clear_tree();
		for (int i = 0; i < vector.size(); i++)
		{
			insert(vector[i]);
		}
	}

	void clear_tree()
	{
		if (root != nullptr)
		{
			std::queue<Node*>coada_de_noduri;
			Node* parinte = root;
			coada_de_noduri.push(parinte);
			while (coada_de_noduri.size() != 0)
			{
				parinte = coada_de_noduri.front();
				coada_de_noduri.pop();
				if (parinte->left != nullptr)
				{
					coada_de_noduri.push(parinte->left);
				}
				if (parinte->right != nullptr)
				{
					coada_de_noduri.push(parinte->right);
				}
				delete parinte;
			}
			root = nullptr;
		}
	}

	bool empty()
	{
		if (root != nullptr)
		{
			return true;
		}
		return false;
	}

	void print2DUtil(Node* root, int space, int COUNT = 10)
	{
		if (root == NULL)
			return;

		space += COUNT;

		print2DUtil(root->right, space);

		std::cout << std::endl;
		for (int i = COUNT; i < space; i++)
			std::cout << " ";
		std::cout << root->key << "\n";

		print2DUtil(root->left, space);
	}

	void print2D()
	{
		print2DUtil(root, 0);
	}

	Node* find(int cheie)
	{
		Node* nod_curent = root;
		while (nod_curent != nullptr)
		{
			if (nod_curent->key == cheie)
			{
				return nod_curent;
			}
			if (cheie < nod_curent->key)
			{
				nod_curent = nod_curent->left;
			}
			else
			{
				nod_curent = nod_curent->right;
			}
		}
		return nullptr;
	}

	void print_tree(int op)
	{
		if (op == 1)
		{
			printPreOrder(root);
		}
		if (op == 2)
		{
			printInOrder(root);
		}
		if (op == 3)
		{
			printPostOrder(root);
		}
		if (op == 4)
		{
			printPeNiveluri();
		}
	}

	void printPreOrder(Node* nod)
	{
		if (nod == nullptr)
		{
			return;
		}
		std::cout << nod->key << " ";
		printPreOrder(nod->left);
		printPreOrder(nod->right);
	}

	void printInOrder(Node* nod)
	{
		if (nod == nullptr)
		{
			return;
		}
		printInOrder(nod->left);
		std::cout << nod->key << " ";
		printInOrder(nod->right);
	}

	void printPostOrder(Node* nod)
	{
		if (nod == nullptr)
		{
			return;
		}

		printPostOrder(nod->left);
		printPostOrder(nod->right);

		std::cout << nod->key << " ";

	}

	void printPeNiveluri()
	{
		Node* parinte;
		std::queue<Node*>coada;
		coada.push(root);
		while (coada.size() != 0)
		{
			std::cout << coada.front()->key << " ";
			if (coada.front()->left != nullptr)
			{
				coada.push(coada.front()->left);
			}
			if (coada.front()->right != nullptr)
			{
				coada.push(coada.front()->right);
			}
			coada.pop();
		}
	}

	Node* getRoot()
	{
		return root;
	}

	Node* getSuccesor(Node* nod)
	{
		if (nod != nullptr)
		{
			Node* Succesor;
			if (nod->right != nullptr)
			{
				Succesor = getMinNode(nod->right);
			}
			else
			{
				Succesor = nod->parent;
				while (Succesor != nullptr && nod == Succesor->right)
				{
					nod = Succesor;
					Succesor = Succesor->parent;
				}
			}
			return Succesor;
		}
		else return nullptr;
	}

	Node* getPredecessor(Node* nod)
	{
		if (nod != nullptr)
		{
			Node* Predecessor;
			if (nod->left != nullptr)
			{
				Predecessor = getMaxNode(nod->left);
			}
			else
			{
				Predecessor = nod->parent;
				while (Predecessor != nullptr && nod == Predecessor->left)
				{
					nod = Predecessor;
					Predecessor = Predecessor->parent;
				}
				return Predecessor;
			}
		}
		else return nullptr;
	}

	Node* getMaxNode(Node* nod_curent = nullptr)
	{
		if (nod_curent == nullptr)
		{
			nod_curent = root;
		}
		Node* max_key = nullptr;
		while (nod_curent != nullptr)
		{
			max_key = nod_curent;
			nod_curent = nod_curent->right;
		}
		return max_key;
	}

	Node* getMinNode(Node* nod_curent = nullptr)
	{
		if (nod_curent == nullptr)
		{
			nod_curent = root;
		}
		Node* min_key = nullptr;
		while (nod_curent != nullptr)
		{
			min_key = nod_curent;
			nod_curent = nod_curent->left;
		}
		return min_key;
	}
};

void menu(AVL s)
{
	int op = 1, element;
	while (op != 0)
	{
		std::cout << std::endl;
		std::cout << "Meniu" << std::endl;
		std::cout << "1. Inserare" << std::endl;
		std::cout << "2. Cautare" << std::endl;
		std::cout << "3. Stergere" << std::endl;
		std::cout << "4. Minim" << std::endl;
		std::cout << "5. Maxim" << std::endl;
		std::cout << "6. Succesor" << std::endl;
		std::cout << "7. Predecesor" << std::endl;
		std::cout << "8. Afisare" << std::endl;
		std::cout << std::endl;

		std::cin >> op;

		std::cout << std::endl;

		switch (op)
		{
		case 1:
			std::cout << "Alege valoarea pe care vrei sa o inserezi: ";
			std::cin >> element;
			s.insert(element);
			break;
		case 2:
			std::cout << "Alege valoarea pe care vrei sa o cauti: ";
			std::cin >> element;
			if (s.find(element) != nullptr)
			{
				std::cout << std::endl;
				std::cout << "Elementul a fost gasit";
				std::cout << std::endl;
			}
			else
			{
				std::cout << std::endl;
				std::cout << "Elementul NU a fost gasit";
				std::cout << std::endl;
			}
			break;
		case 3:
			std::cout << "Alege valoarea pe care vrei sa o stergi: ";
			std::cin >> element;
			s.delete_key(element);
			break;
		case 4:
			std::cout << "Cheia minima din arbore este " << s.getMinNode()->key;
			std::cout << std::endl;
			break;
		case 5:
			std::cout << "Cheia maxima din arbore este " << s.getMaxNode()->key;
			std::cout << std::endl;
			break;
		case 6:
			std::cout << "Citeste cheia pentru care doresti sa afli succesorul: ";
			std::cin >> element;
			std::cout << std::endl;
			if (s.getSuccesor(s.find(element)) != nullptr)
			{
				std::cout << "Succesorul este " << s.getSuccesor(s.find(element))->key;
			}
			else std::cout << "Elementul nu are Succesor";
			std::cout << std::endl;
			break;
		case 7:
			std::cout << "Citeste cheia pentru care doresti sa afli predecesorul: ";
			std::cin >> element;
			std::cout << std::endl;
			if (s.getPredecessor(s.find(element)) != nullptr)
			{
				std::cout << "Succesorul este " << s.getPredecessor(s.find(element))->key;
			}
			else std::cout << "Elementul nu are Predecesor";
			std::cout << std::endl;
			break;
		case 8:
			std::cout << "1. Afisare in PreOrdine" << std::endl;
			std::cout << "2. Afisare in InOrdine" << std::endl;
			std::cout << "3. Afisare in PostOrdine" << std::endl;
			std::cout << "4. Afisare pe Niveluri" << std::endl;
			std::cout << "Alege mod de afisare: ";
			std::cin >> element;
			std::cout << std::endl;
			s.print_tree(element);
			std::cout << std::endl;
			break;
		}
	}
}


int main()
{
	AVL a;
	//menu(a);
	a.insert(46);
	a.insert(2);
	a.insert(89);
	a.insert(65);
	a.insert(88);
	a.print2D();

	return 0;
}