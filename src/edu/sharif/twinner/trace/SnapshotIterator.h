//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef SNAPSHOT_ITERATOR_H
#define SNAPSHOT_ITERATOR_H

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class ExecutionTraceSegment;
class Snapshot;

class SnapshotIterator {
public:
  typedef std::list < ExecutionTraceSegment * >
  ::const_reverse_iterator SegmentListIterator;
  typedef std::list < Snapshot * >::const_iterator SnapshotListIterator;

  typedef std::ptrdiff_t difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Snapshot value_type;
  typedef Snapshot *pointer;
  typedef Snapshot &reference;


private:
  const std::list < ExecutionTraceSegment * > &segments;
  SegmentListIterator segIt;
  SnapshotListIterator snaIt;

public:
  SnapshotIterator (const std::list < ExecutionTraceSegment * > &segments,
      SegmentListIterator segIt = SegmentListIterator (),
      SnapshotListIterator snaIt = SnapshotListIterator ());
  SnapshotIterator (const SnapshotIterator &it);
  ~SnapshotIterator ();

  SnapshotIterator begin () const;
  SnapshotIterator end () const;

  SnapshotIterator &operator++ (); // prefix ++
  SnapshotIterator operator++ (int); // postfix ++

  SnapshotIterator &operator-- (); // prefix --
  SnapshotIterator operator-- (int); // postfix --

  Snapshot &operator* ();
  Snapshot *operator-> ();

  bool operator== (const SnapshotIterator &it) const;
  bool operator!= (const SnapshotIterator &it) const;
};

}
}
}
}

#endif /* SnapshotIterator.h */
