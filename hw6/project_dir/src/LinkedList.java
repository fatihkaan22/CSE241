public class LinkedList<E> extends AbstractList<E> implements List<E>, Queue<E> {
  /**
   * Returns the head (first element) of the queue
   *
   * @return head of the queue
   * @throws IndexOutOfBoundsException if no element left
   */
  @Override
  public E element() {
    return get(0);
  }

  /**
   * Adds element to the list
   *
   * @param e Element to be added
   * @return true
   */
  @Override
  public boolean offer(E e) { // offer intentionally does not throw exception
    add(e);
    return true;
  }

  /**
   * removes the head of the queue
   *
   * @return removed element
   */
  @Override
  public E poll() {
    if (size() <= 0) return null;
    E e = arr[0];
    removeAt(0);
    return e;
  }
}
