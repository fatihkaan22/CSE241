public abstract class AbstractList<E> extends AbstractCollection<E> implements List<E> {

  /**
   * Returns the element at the specified position in this list.
   *
   * @param index index of the desired element
   * @throws IndexOutOfBoundsException if index is illegal
   * @return element
   */
  @Override
  public E get(int index) throws IndexOutOfBoundsException {
    rangeCheck(index);
    return arr[index];
  }

  /**
   * Adds element in the specified position
   *
   * @param index position to be added
   * @param e     element to be added
   * @return true
   * @throws IndexOutOfBoundsException if index is not valid
   */
  @Override
  public boolean add(int index, E e) throws IndexOutOfBoundsException {
    rangeCheckForAdd(index);
    if (size == arr.length) { //array is full
      increaseCapacity();
    }
    if (size > index) {
      enlarge(index);
      arr[index] = e;
      this.size++;
      return true;
    } else { // size equal to index
      arr[size] = e;
      this.size++;
      return true;
    }
  }
}


