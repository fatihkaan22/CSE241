public interface Queue<E> extends Collection<E> {
  /**
   * Adds element to the list
   *
   * @param e Element to be added
   * @return true
   */
  @Override
  boolean add(E e);

  /**
   * Returns the head (first element) of the queue
   *
   * @return head of the queue
   * @throws IndexOutOfBoundsException if no element left
   */
  E element();

  /**
   * Adds element to the list
   *
   * @param e Element to be added
   * @return true
   */
  boolean offer(E e);

  /**
   * removes the head of the queue
   *
   * @return removed element
   */
  E poll();

}
