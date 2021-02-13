public interface Iterator<E> {

  /**
   * Returns true if the iteration has more elements.
   *
   * @return true if cursor on the last element
   */
  boolean hasNext();

  /**
   * Returns the next element in the iteration and advances the iterator.
   *
   * @return the element which cursor points to
   */
  E next();

  /**
   * Removes from the underlying collection the last element returned by this iterator.
   *
   * @throws java.util.NoSuchElementException if element cannot be removed
   * @throws UnsupportedOperationException    if the collection is a set
   */
  void remove();

}
