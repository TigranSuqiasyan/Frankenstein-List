#include "flist.hpp"

namespace spec {

// ---------------------------------------- CONSTRUCTORS AND DESTRUCTOR

template <typename T>
flist<T>::flist() : m_head{}, m_tail{}, m_smallest{}, m_greatest{} {}

template <typename T>
flist<T>::flist(int count) : flist() {
  T temp{};
  while (count--)
    push_back(temp);
}

template <typename T>
flist<T>::flist(int count, const T& value) : flist() {
  while (count--)
    push_back(value);
}

template <typename T>
flist<T>::flist(std::initializer_list<T> ilist) {
  auto begin = ilist.begin();
  auto end = ilist.end();
  while (begin != end) {
    push_back(*(begin++));
  }
}

template <typename T>
flist<T>::flist(const flist<T>& oth) {
  auto curr = oth.m_head;
  while (curr) {
    push_back(curr->val);
    curr = curr->next;
  }
}

template <typename T>
flist<T>::flist(flist<T>&& oth) 
  : m_head{oth.m_head},
    m_tail{oth.m_tail},
    m_smallest{oth.m_smallest},
    m_greatest{oth.m_greatest} {
  oth.m_head = nullptr;
  oth.m_tail = nullptr;
  oth.m_smallest = nullptr;
  oth.m_greatest = nullptr;
}

template <typename T>
flist<T>::~flist() {
  clear(m_head);
}


// ----------------------------------------------------- OPERATORS  

template <typename T>
flist<T>& flist<T>::operator=(const flist<T>& rhs) {
  flist<T> temp(rhs);
  swap(temp);
  return *this;
}

template <typename T>
flist<T>& flist<T>::operator=(flist<T>&& oth) {
  clear();
  m_head = oth.m_head;
  m_tail = oth.m_tail;
  m_smallest = oth.m_smallest;
  m_greatest = oth.m_greatest;
  oth.m_head = nullptr;
  oth.m_tail = nullptr;
  oth.m_smallest = nullptr;
  oth.m_greatest = nullptr;
  return *this;
}

template <typename T>
flist<T>& flist<T>::operator+=(const flist<T>& rhs) {
  auto curr = rhs.m_head;
  while (curr) {
    push_back(curr->val);
    curr = curr->next;
  }
  return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const flist<T>& list) {
  auto curr = list.m_head;
  while (curr) {
    os << curr->val << " ";
    curr = curr->next;
  }
  return os;
}

template <typename T>
flist<T> operator+(const flist<T>& lhs, const flist<T>& rhs) {
  flist<T> result;
  auto curr = lhs.m_head;
  while (curr) {
    result.push_back(curr->val);
    curr = curr->next;
  }
  curr = rhs.m_head;
  while (curr) {
    result.push_back(curr->val);
    curr = curr->next;
  }
  return result;
}

template <typename T>
bool operator==(const flist<T>& lhs, const flist<T>& rhs) {
  if (lhs.size() != rhs.size())
    return false;
  auto curr1 = lhs.m_head;
  auto curr2 = rhs.m_head;
  while (curr1) {
    if (curr1->val != curr2->val)
      return false;
    curr1 = curr1->next;
    curr2 = curr2->next;
  }
  return true;
}

template <typename T>
bool operator!=(const flist<T>& lhs, const flist<T>& rhs) {
  return !(lhs == rhs);
}

template <typename T>
bool operator<(const flist<T>& lhs, const flist<T>& rhs) {
  auto curr1 = lhs.m_head;
  auto curr2 = rhs.m_head;
  while (curr1 && curr2) {
    if (curr1->val < curr2->val)
      return true;
    if (curr1->val > curr2->val)
      return false;
    curr1 = curr1->next;
    curr2 = curr2->next;
  }
  if (curr1) return false;
  return true;
}

template <typename T>
bool operator>(const flist<T>& lhs, const flist<T>& rhs) {
  return !(lhs == rhs) && !(lhs < rhs);
}

template <typename T>
bool operator<=(const flist<T>& lhs, const flist<T>& rhs) {
  return !(lhs > rhs);
}

template <typename T>
bool operator>=(const flist<T>& lhs, const flist<T>& rhs) {
  return !(lhs < rhs);
}

// -------------------------------------------- NON-MODIFYONG DUNCTIONS 

template <typename T>
const T& flist<T>::front() const {
  if (m_head)
    return m_head->val;
  throw std::out_of_range("the container is empty");
}

template <typename T>
const T& flist<T>::back() const {
  if (m_tail)
    return m_tail->val;
  throw std::out_of_range("the container is empty");
}

template <typename T>
bool flist<T>::empty() const {
  return m_head != nullptr;
}

template <typename T>
size_t flist<T>::size() const {
  size_t count{};
  auto curr = m_head;
  while (curr) {
    ++count;
    curr = curr->next;
  }
  return count;
}

//  --------------------------------------------- MODIFYING FUNCTIONS

template <typename T>
void flist<T>::clear() {
  clear(m_head);
  m_head = nullptr;
  m_tail = nullptr;
  m_smallest = nullptr;
  m_greatest = nullptr;
}

template <typename T>
void flist<T>::insert(size_t pos, const T& value) {
  if (pos < 0 || pos > size())
    throw std::invalid_argument("cannot insert in that position");
  if (pos == 0) {
    push_front(value);
    return;
  }
  if (pos == size()) {
    push_back(value);
    return;
  }
  auto curr = m_head;
  while (pos--) {
    curr = curr->next;
  }
  node* tmp = new node(value, curr->prev, curr);
  curr->prev = tmp;
  tmp->prev->next = tmp;
  fix_order(tmp);
}

template <typename T>
void flist<T>::push_front(const T& value) {
  if (m_head == nullptr) {
    m_head = new node(value);
    m_tail = m_head;
    m_smallest = m_head;
    m_greatest = m_head;
    return;
  }
  node* tmp = new node(value);
  tmp->next = m_head;
  m_head->prev = tmp;
  m_head = tmp;
  fix_order(tmp);
}

template <typename T>
void flist<T>::push_back(const T& value) {
  if (m_tail == nullptr) {
    m_head = new node(value);
    m_tail = m_head;
    m_smallest = m_head;
    m_greatest = m_head;
    return;
  }
  node* tmp = new node(value);
  tmp->prev = m_tail;
  m_tail->next = tmp;
  m_tail = tmp;
  fix_order(tmp);
}

template <typename T>
void flist<T>::pop_front() {
  if (size() == 0)
    throw std::out_of_range("no more elements left");
  if (size() == 1) {
    delete m_head;
    m_head = nullptr;
    m_tail = nullptr;
    m_smallest = nullptr;
    m_greatest = nullptr;
    return;
  }
  auto tmp = m_head;
  m_head = m_head->next;
  m_head->prev = nullptr;
  fix_before_removal(tmp);
}

template <typename T>
void flist<T>::pop_back() {
  if (size() == 0)
    throw std::out_of_range("no more elements left");
  if (size() == 1) {
    delete m_head;
    m_head = nullptr;
    m_tail = nullptr;
    m_smallest = nullptr;
    m_greatest = nullptr;
    return;
  }
  auto tmp = m_tail;
  m_tail = m_tail->prev;
  m_tail->next = nullptr;
  fix_before_removal(tmp);
}

template <typename T>
void flist<T>::erase(size_t pos) {
  if (pos < 0 || pos >= size())
    throw std::invalid_argument("there is no element in that position");
  if (pos == 0) {
    pop_front();
    return;
  }
  if (pos == size() - 1) {
    pop_back();
    return;
  }
  auto tmp = m_head;
  while (pos--) {
    tmp = tmp->next;
  }
  auto next = tmp->next;
  next->prev = tmp->prev;
  tmp->prev->next = next;
  fix_before_removal(tmp);
}

template <typename T>
void flist<T>::swap(flist<T>& oth) {
  std::swap(m_head, oth.m_head);
  std::swap(m_tail, oth.m_tail);
  std::swap(m_smallest, oth.m_smallest);
  std::swap(m_greatest, oth.m_greatest);
}

template <typename T>
void flist<T>::resize(size_t count) {
  T temp{};
  resize(count, temp);
}

template <typename T>
void flist<T>::resize(size_t count, const T& value) {
  if (count < 0)
    throw std::invalid_argument("can't resize to negative count");
  if (count == 0) {
    clear();
    return;
  }
  if (count == size())
    return;
  int s = size();
  if (count > s) {
    for (int i{}; i < count - s; ++i)
      push_back(value);
    return;
  }
  for (int i{}; i < s - count; ++i)
    pop_back();
}

// ---------------------------------------------------------- OPERATIONS

template <typename T>
void flist<T>::splice(size_t pos, flist<T>& oth) {
  if (pos < 0 || pos > size())
    throw std::invalid_argument("can't splice at that position");
  auto curr = m_head;
  for (int i{}; i < pos; ++i)
    curr = curr->next;
  int index{};
  while (oth.m_head) {
    insert(pos + index, oth.front());
    oth.pop_front();
    ++index;
  }
}

template <typename T>
void flist<T>::remove(const T& value) {
  if (m_head == nullptr)
    return;
  auto curr = m_head;
  size_t index{};
  while (curr) {
    if (curr->val == value) {
      curr = curr->next;
      erase(index);
    }
    else {
      ++index;
      curr = curr->next;
    }
  }
}

template <typename T>
template <typename UnaryPredicate>
void flist<T>::remove_if(UnaryPredicate op) {
  if (m_head == nullptr)
    return;
  auto curr = m_head;
  size_t index{};
  while (curr) {
    if (op(curr->val)) {
      curr = curr->next;
      erase(index);
    }
    else {
      ++index;
      curr = curr->next;
    }
  }
}

template <typename T>
void flist<T>::unique() {
  if (m_head == nullptr || m_head->next == nullptr)
    return;
  while (m_head->next && m_head->next->val == m_head->val)
    erase(1);
  size_t index{};
  auto curr = m_head;
  while (curr && curr->next) {
    if (curr->next->val == curr->val)
      erase(index + 1);
    else {
      ++index;
      curr = curr->next;
    }
  }
}

// --------------------------------------------------- SPECIFIC FUNCTIOS

template <typename T>
void flist<T>::traverse(void (*fptr)(T&)) {
  auto curr = m_head;
  while (curr) {
    fptr(curr->val);
    curr = curr->next;
  }
}

template <typename T>
void flist<T>::print_sorted() const {
  auto curr = m_smallest;
  while (curr) {
    std::cout << curr->val << " ";
    curr = curr->asc;
  }
}

// ---------------------------------------------------- HELPER FUNCTIONS

template <typename T>
void flist<T>::clear(node* head) {
  if (head == nullptr)
    return;
  clear(head->next);
  delete head;
  head = nullptr;
}

template <typename T>
void flist<T>::fix_order(node* new_node) {
  if (size() == 0) {
    m_smallest == nullptr;
    m_greatest = nullptr;
    return;
  }
  if (size() == 1) {
    m_smallest = new_node;
    m_greatest = new_node;
    new_node->asc = nullptr;
    new_node->desc = nullptr;
    return;
  }
  if (new_node->val <= m_smallest->val) {
    new_node->asc = m_smallest;
    m_smallest->desc = new_node;
    m_smallest = new_node;
    return;
  }
  if (new_node->val >= m_greatest->val) {
    new_node->desc = m_greatest;
    m_greatest->asc = new_node;
    m_greatest = new_node;
    return;
  }
  auto curr = m_smallest;
  while (curr->val < new_node->val) {
    curr = curr->asc;
  }
  auto tmp = curr->desc;
  tmp->asc = new_node;
  new_node->desc = tmp;
  new_node->asc = curr;
  curr->desc = new_node;
}

template <typename T>
void flist<T>::fix_before_removal(node* tmp) {
  if (m_smallest == tmp) {
    m_smallest = tmp->asc;
    m_smallest->desc = nullptr;
    delete tmp;
    return;
  }
  if (m_greatest == tmp) {
    m_greatest = tmp->desc;
    m_greatest->asc = nullptr;
    delete tmp;
    return;
  }
  auto next_ascending = tmp->asc;
  next_ascending->desc = tmp->desc;
  tmp->desc->asc = next_ascending;
  delete tmp;
  next_ascending = nullptr;
}

} // spec

int main() {
  spec::flist<int> f{1, 1,2, 2,8,6,3, 3,5,4,7,-4,-8, -8};
  spec::flist<int> g{1,6,9,7,4,5,3};
  
  f.splice(14, g);
  f.remove(-9);
  f.traverse([](int& a) {++a; return;});
  f.remove_if([](int a) -> bool{return a % 2 == 0;});

  std::cout << f << std::endl;
  f.print_sorted();
  std::cout << std::endl;

}