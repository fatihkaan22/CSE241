import java.util.NoSuchElementException;

public abstract class AbstractCollection<E> implements Collection<E> {
  /**
   * initial capacity of the array
   */
  static final int INITIAL_CAPACITY = 10;
  protected E[] arr;
  /**
   * number of elements
   */
  protected int size;

  /**
   * Constructs an empty array with size 0
   */
  public AbstractCollection() {
    this.size = 0;
    this.arr = (E[]) new Object[INITIAL_CAPACITY];
  }

  /**
   * Returns info of index and size
   *
   * @param index index trying to reach
   * @return info of index and size
   */
  private String outOfBoundsMsg(int index) {
    return "Index: " + index + ", Size: " + this.size;
  }


  /**
   * Checks if adding is legal
   *
   * @param index index to be attempted to add
   * @throws IndexOutOfBoundsException if index not in range
   */
  protected void rangeCheckForAdd(int index) throws IndexOutOfBoundsException {
    if (index < 0 || index > this.size) {
      throw new IndexOutOfBoundsException(this.outOfBoundsMsg(index));
    }
  }

  /**
   * Checks if removing is legal
   *
   * @param index index to be attempted to add
   * @throws IndexOutOfBoundsException if index not in range
   */
  protected void rangeCheck(int index) {
    if (index < 0 || index >= this.size) {
      throw new IndexOutOfBoundsException(this.outOfBoundsMsg(index));
    }
  }

  /**
   * Returns true if the collection contains given element, false otherwise
   *
   * @param e element to be checked
   * @return true, if the collection contains given element, false otherwise
   */
  @Override
  public boolean contains(E e) {
    for (int i = 0; i < size; i++) {
      if (arr[i].equals(e)) return true;
    }
    return false;
  }

  /**
   * @return true if the size of the collection is 0, false otherwise
   */
  @Override
  public boolean isEmpty() {
    return size == 0;
  }

  /**
   * Removes element from the collection, if the size is half of the capacity, decreases the capacity
   *
   * @param e element to be removed
   * @return true, if element is removed
   */
  @Override
  public boolean remove(E e) {
    if ((arr.length > INITIAL_CAPACITY) && size < arr.length / 2)
      decreaseCapacity();
    for (int i = 0; i < size; i++) {
      if (arr[i].equals(e)) {
        // remove
        shrink(i);
        return true;
      }
    }
    return false;
  }


  /**
   * Removes element at the specified position in this list.
   *
   * @param index index of the element to be removed
   */
  protected void removeAt(int index) { // needed for iterator's remove()
    rangeCheck(index);
    shrink(index);
  }

  /**
   * Doubles the capacity of the array
   */
  protected void increaseCapacity() {
    E[] newArr = (E[]) new Object[arr.length * 2];
    System.arraycopy(arr, 0, newArr, 0, size);
    this.arr = newArr;
  }

  /**
   * Decreases the capacity of the array to half
   */
  protected void decreaseCapacity() {
    E[] newArr = (E[]) new Object[arr.length / 2];
    System.arraycopy(arr, 0, newArr, 0, size);
    this.arr = newArr;
  }

  /**
   * Adds element to the collection
   *
   * @param e Element to be added
   * @return true
   */
  @Override
  public boolean add(E e) {
    if (arr.length == size) {
      increaseCapacity();
    }
    arr[size] = e;
    this.size++;
    return true;
  }


  /**
   * adds all the element in the given collection, to the collection
   *
   * @param c collection
   * @return true
   */
  @Override
  public boolean addAll(Collection<E> c) {
    Iterator<E> iter = c.iterator();
    while (iter.hasNext()) {
      add(iter.next());
    }
    return true;
  }

  /**
   * Returns the number of elements in this collection.
   *
   * @return size of the collection
   */
  @Override
  public int size() {
    return size;
  }


  /**
   * removes the elements of the collection, if those element also is in the given collection
   *
   * @param c collection whose elements are to be removed
   * @return true
   */
  @Override
  public boolean removeAll(Collection<E> c) {
    Iterator<E> iter = c.iterator();
    while (iter.hasNext()) {
      remove(iter.next());
    }
    return true;
  }

  /**
   * if all elements in the given collection are contained by this collection, return true
   *
   * @param c given collection
   * @return if contains all elements of the given collection
   */
  @Override
  public boolean containsAll(Collection<E> c) {
    Iterator<E> iter = c.iterator();
    while (iter.hasNext()) {
      if (!contains(iter.next())) return false;
    }
    return true;
  }


  /**
   * remove all elements in the collection, and set its capacity to the initial capacity
   */
  @Override
  public void clear() {
    this.arr = (E[]) new Object[INITIAL_CAPACITY];
    size = 0;
  }

  /**
   * returns all elements of the collection as comma separated string
   *
   * @return elements of the collection
   */
  @Override
  public String toString() {
    if (size <= 0) return "[]";
    StringBuilder sb = new StringBuilder();
    sb.append('[');
    for (int i = 0; i < size; i++) {
      sb.append(arr[i]).append(',');
    }
    sb.deleteCharAt(sb.lastIndexOf(","));
    sb.append(']');
    return sb.toString();
  }

  /**
   * Returns an iterator over the collection
   *
   * @return iterator
   */
  @Override
  public Iterator<E> iterator() {
    return new InnerIter();
  }

  /**
   * Retains only the elements in this collection that are contained in the specified collection
   *
   * @param c collection whose element to be retained
   * @return if retained any element
   */
  @Override
  public boolean retainAll(Collection<E> c) {
    int oldSize = size;

    Iterator<E> iter = c.iterator();
    while (iter.hasNext()) {
      remove(iter.next());
    }
    return oldSize != size;
  }

  /**
   * Opens up space for adding new element middle of the array by shifting
   *
   * @param startIndex index will be emptied
   */
  protected void enlarge(int startIndex) {
    if (size + 1 - startIndex >= 0)
      System.arraycopy(arr, startIndex, arr, startIndex + 1, size - startIndex);
  }

  /**
   * Shifts element by 1 after specified index after remove
   *
   * @param index null index
   */
  protected void shrink(int index) {
    //FIXME: maybe do it manually?
    System.arraycopy(arr, index + 1, arr, index, size - index - 1);
    size--;
    arr[size] = null;
  }

  /**
   * Iterator of collections
   */
  private class InnerIter implements Iterator<E> {
    int cursor = 0;
    /**
     * index of last returned element
     */
    int lastRet = -1;

    /**
     * Returns true if the iteration has more elements.
     *
     * @return true if cursor on the last element
     */
    public boolean hasNext() {
      return cursor != size;
    }

    /**
     * Returns the next element in the iteration and advances the iterator.
     *
     * @return the element which cursor points to
     */
    public E next() {
      lastRet = cursor;
      return AbstractCollection.this.arr[cursor++];
    }

    /**
     * Removes from the underlying collection the last element returned by this iterator.
     *
     * @throws NoSuchElementException        if element cannot be removed
     * @throws UnsupportedOperationException if the collection is a set
     */
    public void remove() throws UnsupportedOperationException, NoSuchElementException {
      if (AbstractCollection.this instanceof Set)
        throw new UnsupportedOperationException("Collection is set");
      if (cursor <= 0 || lastRet == -1)
        throw new NoSuchElementException();
      AbstractCollection.this.removeAt(lastRet);
      lastRet = -1;
      cursor--;
    }

  }

}
