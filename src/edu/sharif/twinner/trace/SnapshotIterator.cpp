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

#include "SnapshotIterator.h"

#include "ExecutionTraceSegment.h"
#include "Snapshot.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SnapshotIterator::SnapshotIterator (
    const std::list < ExecutionTraceSegment * > &_segments,
    SegmentListIterator _segIt, SnapshotListIterator _snaIt) :
    segments (_segments), segIt (_segIt), snaIt (_snaIt) {
}

SnapshotIterator::SnapshotIterator (const SnapshotIterator &it) :
    segments (it.segments), segIt (it.segIt), snaIt (it.snaIt) {
}

SnapshotIterator::~SnapshotIterator () {
}

SnapshotIterator SnapshotIterator::begin () const {
  SegmentListIterator it = segments.rbegin ();
  return SnapshotIterator (segments, it, (*it)->getSnapshots ().begin ());
}

SnapshotIterator SnapshotIterator::end () const {
  SegmentListIterator it = segments.rend ();
  return SnapshotIterator (segments, it);
}

SnapshotIterator &SnapshotIterator::operator++ () {
  if (++snaIt == (*segIt)->getSnapshots ().end ()) {
    ++segIt;
    if (segIt != segments.rend ()) {
      snaIt = (*segIt)->getSnapshots ().begin ();
    }
  }
  return *this;
}

SnapshotIterator SnapshotIterator::operator++ (int) {
  SnapshotIterator it (*this);
  operator++ ();
  return it;
}

SnapshotIterator &SnapshotIterator::operator-- () {
  if (segIt == segments.rend ()
      || snaIt == (*segIt)->getSnapshots ().begin ()) {
    --segIt;
    snaIt = (*segIt)->getSnapshots ().end ();
  }
  --snaIt;
  return *this;
}

SnapshotIterator SnapshotIterator::operator-- (int) {
  SnapshotIterator it (*this);
  operator-- ();
  return it;
}

Snapshot &SnapshotIterator::operator* () {
  return **snaIt;
}

Snapshot *SnapshotIterator::operator-> () {
  return *snaIt;
}

bool SnapshotIterator::operator== (const SnapshotIterator &it) const {
  return segIt == it.segIt
      && (segIt == segments.rend () || snaIt == it.snaIt);
}

bool SnapshotIterator::operator!= (const SnapshotIterator &it) const {
  return !operator== (it);
}

}
}
}
}
