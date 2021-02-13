public class Main {
  public static void main(String[] args) {

    System.out.println("==== Test with Integers ====");
    System.out.println("==== ArrayList Test ====");
    ArrayList<Integer> arrayList = new ArrayList<>();
    System.out.println("- add elements");
    arrayList.add(3);
    arrayList.add(5);
    arrayList.add(4);
    arrayList.add(5);
    System.out.println(arrayList);
    System.out.println("- print size");
    System.out.println(arrayList.size());
    System.out.println("- iterate with iterator and remove 5");
    Iterator<Integer> iter = arrayList.iterator();
    while (iter.hasNext()) {
      Integer i = iter.next();
      if (i.equals(5))
        iter.remove();
    }
    System.out.println(arrayList);
    System.out.println("- remove 4 as element");
    arrayList.remove(4);
    System.out.println(arrayList);
    System.out.println("- contains(3)");
    System.out.println(arrayList.contains(3));
    System.out.println("- contains(7)");
    System.out.println(arrayList.contains(7));
    System.out.println("- add(0,8)");
    arrayList.add(0, 8);
    System.out.println(arrayList);


    System.out.println("==== LinkedList Test ====");
    LinkedList<Integer> linkedList = new LinkedList<>();
    System.out.println("- offer/add");
    linkedList.offer(0);
    linkedList.add(1);
    linkedList.offer(2);
    linkedList.add(7);
    System.out.println(linkedList);
    System.out.println("- element()");
    System.out.println(linkedList.element());
    System.out.println("- poll()");
    linkedList.poll();
    System.out.println(linkedList);
    System.out.println("- size()");
    System.out.println(linkedList.size());
    System.out.println("- containts(2)");
    System.out.println(linkedList.contains(2));
    System.out.println("- linkedList.addAll(arrayList)");
    linkedList.addAll(arrayList);
    System.out.println(linkedList);
    System.out.println("- linkedList.containsAll(arrayList)");
    System.out.println(linkedList.containsAll(arrayList));

    System.out.println("==== HashSet Test ====");
    HashSet<Integer> hashSet = new HashSet<>();
    System.out.println("- hashSet.addAll(arrayList)");
    hashSet.addAll(linkedList);
    System.out.println(hashSet);
    System.out.println("- hashSet.addAll(arrayList) to show no duplicate elements");
    hashSet.addAll(linkedList);
    System.out.println(hashSet);
    System.out.println("- hashSet.removeAll(arrayList)");
    hashSet.removeAll(arrayList);
    System.out.println(hashSet);
    System.out.println("- hashSet.retainAll(linkedList)");
    hashSet.retainAll(linkedList);
    System.out.println(hashSet);
    System.out.println("- hashSet.add(1)");
    hashSet.add(1);
    System.out.println("- hashSet.add(2)");
    hashSet.add(2);
    System.out.println("- hashSet.add(2) second time");
    hashSet.add(2);
    System.out.println(hashSet);
    System.out.println("- remove(1)");
    hashSet.remove(1);
    System.out.println(hashSet);
    System.out.println("- hashSet.isEmpty()");
    System.out.println(hashSet.isEmpty());
    System.out.println("- hashSet.clear()");
    hashSet.clear();
    System.out.println(hashSet);
    System.out.println("- hashSet.isEmpty()");
    System.out.println(hashSet.isEmpty());


    System.out.println("==== Test with Strings ====");
    System.out.println("==== ArrayList Test ====");
    ArrayList<String> arrayListS = new ArrayList<>();
    System.out.println("- add elements");
    arrayListS.add("three");
    arrayListS.add("five");
    arrayListS.add("four");
    arrayListS.add("five");
    System.out.println(arrayListS);
    System.out.println("- print size");
    System.out.println(arrayListS.size());
    System.out.println("- iterate with iterator and remove \"five\"");
    Iterator<String> iterS = arrayListS.iterator();
    while (iterS.hasNext()) {
      String s = iterS.next();
      if (s.equals("five"))
        iterS.remove();
    }
    System.out.println(arrayListS);
    System.out.println("- remove \"four\"");
    arrayListS.remove("four");
    System.out.println(arrayListS);
    System.out.println("- contains(\"three\")");
    System.out.println(arrayListS.contains("three"));
    System.out.println("- contains(\"seven\")");
    System.out.println(arrayListS.contains("seven"));
    System.out.println("- add(0,\"eight\")");
    arrayListS.add(0, "eight");
    System.out.println(arrayListS);


    System.out.println("==== LinkedList Test ====");
    LinkedList<String> linkedListS = new LinkedList<>();
    System.out.println("- offer/add");
    linkedListS.offer("zero");
    linkedListS.add("one");
    linkedListS.offer("two");
    linkedListS.add("seven");
    System.out.println(linkedListS);
    System.out.println("- element()");
    System.out.println(linkedListS.element());
    System.out.println("- poll()");
    linkedListS.poll();
    System.out.println(linkedListS);
    System.out.println("- size()");
    System.out.println(linkedListS.size());
    System.out.println("- contains(\"two\")");
    System.out.println(linkedListS.contains("two"));
    System.out.println("- linkedListS.addAll(arrayListS)");
    linkedListS.addAll(arrayListS);
    System.out.println(linkedListS);

    System.out.println("==== HashSet Test ====");
    HashSet<String> hashSetS = new HashSet<>();
    System.out.println("- hashSetS.addAll(arrayListS)");
    hashSetS.addAll(linkedListS);
    System.out.println(hashSetS);
    System.out.println("- hashSetS.addAll(arrayListS) to show no duplicate elements");
    hashSetS.addAll(linkedListS);
    System.out.println(hashSetS);
    System.out.println("- hashSetS.removeAll(arrayListS)");
    hashSetS.removeAll(arrayListS);
    System.out.println(hashSetS);
    System.out.println("- hashSetS.retainAll(linkedListS)");
    hashSetS.retainAll(linkedListS);
    System.out.println(hashSetS);
    System.out.println("- hashSetS.add(\"one\")");
    hashSetS.add("one");
    System.out.println("- hashSetS.add(\"two\")");
    hashSetS.add("two");
    System.out.println("- hashSetS.add(\"two\") second time");
    hashSetS.add("two");
    System.out.println(hashSetS);
    System.out.println("- remove(one)");
    hashSetS.remove("one");
    System.out.println(hashSetS);

    try {
      System.out.println("- exception test: iterator.remove() on set");
      Iterator<String> iterHashSetS = hashSetS.iterator();
      iterHashSetS.remove();
    } catch (UnsupportedOperationException e) {
      System.err.println(e);
    }

    try {
      System.out.println("- exception test: arrayListS.add(1000,\"eight\")");
      arrayListS.add(1000, "eight"); // throws exception
    } catch (IndexOutOfBoundsException e) {
      System.err.println(e);
    }

    try {
      System.out.println("- exception test: linkedList.add(-4,8)");
      linkedList.add(-4, 8); // throws exception
    } catch (Exception e) {
      System.err.println(e);
    }

  }

}
