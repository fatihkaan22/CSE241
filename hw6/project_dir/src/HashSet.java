public class HashSet<E> extends AbstractCollection<E> implements Set<E> {

  /**
   * Adds element to the set if already not in the set
   *
   * @param e Element to be added
   * @return false if already in the list, true if added
   */
  @Override
  public boolean add(E e) {
    for (int i = 0; i < size; i++) {
      if (arr[i].equals(e)) return false;
    }
    return super.add(e);
  }
}