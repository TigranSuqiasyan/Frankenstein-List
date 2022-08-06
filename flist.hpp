#ifndef FRANKENSTEIN_LIST_H
#define FRANKENSTEIN_LIST_H

#include <iostream>
#include <initializer_list>

namespace spec {

template <typename T>
class flist {

private:
  struct node;

public:
  flist(); 
  flist(int); 
  flist(int n, const T&); 
  flist(std::initializer_list<T>); 
  flist(const flist<T>&); 
  flist(flist<T>&&); 
  ~flist(); 

public:
  flist<T>& operator=(const flist<T>&); 
  flist<T>& operator=(flist<T>&&); 
  flist<T>& operator+=(const flist<T>&); 
  template <typename E> 
    friend flist<E> operator+(const flist<E>&, const flist<E>&); 
  template <typename E> 
    friend std::ostream& operator<<(std::ostream&, const flist<E>&);
  template <typename E> 
    friend bool operator==(const flist<E>&, const flist<E>&);
  template <typename E> 
    friend bool operator!=(const flist<E>&, const flist<E>&); 
  template <typename E> 
    friend bool operator< (const flist<E>&, const flist<E>&);
  template <typename E> 
    friend bool operator> (const flist<E>&, const flist<E>&); 
  template <typename E> 
    friend bool operator<=(const flist<E>&, const flist<E>&); 
  template <typename E> 
    friend bool operator>=(const flist<E>&, const flist<E>&); 

public:
  const T& front() const;  
  const T& back() const;  
  bool empty() const;  
  size_t size() const;  
  
public:
  void clear(); 
  void insert(size_t, const T&); 
  void erase(size_t); 
  void push_back(const T&); 
  void push_front(const T&); 
  void pop_back();  
  void pop_front(); 
  void resize(size_t); 
  void resize(size_t, const T&);
  void swap(flist<T>&); 

public:
  void splice(size_t, flist<T>&);
  void remove(const T&); 
  template <typename UnaryPredicate> 
    void remove_if(UnaryPredicate op);
  void unique(); 
  
public:
  void traverse(void (*fptr)(T&)); 
  void print_sorted() const;

private:
  void clear(node*);
  void fix_order(node*);
  void fix_before_removal(node*); 

private:
  struct node {
    node() = default;
    node(const T& v) 
      : val{v}, prev{}, next{}, asc{}, desc{} {}
    node(const T& v, node* p, node* n) 
      : val{v}, prev{p}, next{n}, asc{}, desc{} {}
    node(const T& v, node* p, node* n, node* a, node*d)
      : val{v}, prev{p}, next{n}, asc{a}, desc{d} {}
    node(const node& oth) = default;
    node(node&& oth) 
      : val{std::move(oth.val)},
        prev{std::move(oth.prev)},
        next{std::move(oth.next)},
        asc{std::move(oth.asc)},
        desc{std::move(oth.desc)} {
    }
    node& operator=(const node&) = default;
    node& operator=(node&& oth) {
      val = std::move(oth.val);
      prev = std::move(oth.prev);
      next = std::move(oth.next);
      asc = std::move(oth.asc);
      desc = std::move(oth.desc);
    }
    ~node() = default;

    T val{};
    node* prev{};
    node* next{};
    node* asc{};
    node* desc{};
  };
  node* m_head{};
  node* m_tail{};
  node* m_smallest{};
  node* m_greatest{};
};

} // spec

#endif // FRANKENSTEIN_LIST_H