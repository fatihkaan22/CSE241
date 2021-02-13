public interface List<E> extends Collection<E> {
  /**
   * Returns the element at the specified position in this list.
   *
   * @param index index of the desired element
   * @return element
   */
  E get(int index);


  /**
   * Adds element in the specified position
   *
   * @param index position to be added
   * @param e     element to be added
   * @return false if array is full, true otherwise
   * @throws IllegalStateException if current size of the array is not equal to index
   */
  boolean add(int index, E e);
}
