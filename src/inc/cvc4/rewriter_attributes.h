/*********************                                                        */
/*! \file rewriter_attributes.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Rewriter attributes
 **
 ** Rewriter attributes.
 **/

#include "cvc4_private.h"

#pragma once

#include "expr/attribute.h"

namespace CVC4 {
namespace theory {

template <bool pre, theory::TheoryId theoryId>
struct RewriteCacheTag {};

template <theory::TheoryId theoryId>
struct RewriteAttibute {

  typedef expr::Attribute< RewriteCacheTag<true, theoryId>, Node> pre_rewrite;
  typedef expr::Attribute< RewriteCacheTag<false, theoryId>, Node> post_rewrite;

  /**
   * Get the value of the pre-rewrite cache.
   */
  static Node getPreRewriteCache(TNode node) throw() {
    Node cache;
    if (node.hasAttribute(pre_rewrite())) {
      node.getAttribute(pre_rewrite(), cache);
    } else {
      return Node::null();
    }
    if (cache.isNull()) {
      return node;
    } else {
      return cache;
    }
  }

  /**
   * Set the value of the pre-rewrite cache.
   */
  static void setPreRewriteCache(TNode node, TNode cache) throw() {
    Trace("rewriter") << "setting pre-rewrite of " << node << " to " << cache << std::endl;
    Assert(!cache.isNull());
    if (node == cache) {
      node.setAttribute(pre_rewrite(), Node::null());
    } else {
      node.setAttribute(pre_rewrite(), cache);
    }
  }

  /**
   * Get the value of the post-rewrite cache.
   * none).
   */
  static Node getPostRewriteCache(TNode node) throw() {
    Node cache;
    if (node.hasAttribute(post_rewrite())) {
      node.getAttribute(post_rewrite(), cache);
    } else {
      return Node::null();
    }
    if (cache.isNull()) {
      return node;
    } else {
      return cache;
    }
  }

  /**
   * Set the value of the post-rewrite cache.  v cannot be a null Node.
   */
  static void setPostRewriteCache(TNode node, TNode cache) throw() {
    Assert(!cache.isNull());
    Trace("rewriter") << "setting rewrite of " << node << " to " << cache << std::endl;
    if (node == cache) {
      node.setAttribute(post_rewrite(), Node::null());
    } else {
      node.setAttribute(post_rewrite(), cache);
    }
  }
};/* struct RewriteAttribute */

}/* CVC4::theory namespace */
}/* CVC4 namespace */
