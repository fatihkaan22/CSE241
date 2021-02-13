public interface Collection<E> {

  /**
   * Returns an iterator over the collection
   *
   * @return iterator
   */
  Iterator<E> iterator();

  /**
   * Adds element to the collection
   *
   * @param e Element to be added
   * @return true
   */
  boolean add(E e);

  /**
   * adds all the element in the given collection, to the collection
   *
   * @param c collection
   * @return true
   */
  boolean addAll(Collection<E> c);

  /**
   * remove all elements in the collection, and set its capacity to the initial capacity
   */
  void clear();

  /**
   * Returns true if the collection contains given element, false otherwise
   *
   * @param e element to be checked
   * @return true, if the collection contains given element, false otherwise
   */
  boolean contains(E e);

  /**
   * if all elements in the given collection are contained by this collection, return true
   *
   * @param c given collection
   * @return if contains all elements of the given collection
   */
  boolean containsAll(Collection<E> c);

  /**
   * Returns true if this collection contains no elements
   *
   * @return true if the size of the collection is 0, false otherwise
   */
  boolean isEmpty();

  /**
   * Removes element from the collection, if the size is half of the capacity, decreases the capacity
   *
   * @param e element to be removed
   * @return true, if element is removed
   */
  boolean remove(E e);

  /**
   * removes the elements of the collection, if those element also is in the given collection
   *
   * @param c collection whose elements are to be removed
   * @return true
   */
  boolean removeAll(Collection<E> c);

  /**
   * Retains only the elements in this collection that are contained in the specified collection
   *
   * @param c collection whose element to be retained
   * @return if retained any element
   */
  boolean retainAll(Collection<E> c);

  /**
   * Returns the number of elements in this collection.
   *
   * @return size of the collection
   */
  int size();
}
