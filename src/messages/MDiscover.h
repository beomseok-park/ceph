// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */


#ifndef CEPH_MDISCOVER_H
#define CEPH_MDISCOVER_H

#include "msg/Message.h"
#include "include/filepath.h"

#include <vector>
#include <string>
using namespace std;


class MDiscover : public Message {
  inodeno_t       base_ino;          // 1 -> root
  frag_t          base_dir_frag;

  snapid_t        snapid;
  filepath        want;   // ... [/]need/this/stuff
  inodeno_t       want_ino;

  bool want_base_dir;
  bool want_xlocked;

 public:
  inodeno_t get_base_ino() { return base_ino; }
  frag_t    get_base_dir_frag() { return base_dir_frag; }
  snapid_t  get_snapid() { return snapid; }

  filepath& get_want() { return want; }
  inodeno_t get_want_ino() { return want_ino; }
  const string& get_dentry(int n) { return want[n]; }

  bool wants_base_dir() { return want_base_dir; }
  bool wants_xlocked() { return want_xlocked; }
  
  void set_base_dir_frag(frag_t f) { base_dir_frag = f; }

  MDiscover() { }
  MDiscover(inodeno_t base_ino_,
	    frag_t base_frag_,
	    snapid_t s,
            filepath& want_path_,
	    inodeno_t want_ino_,
            bool want_base_dir_ = true,
	    bool discover_xlocks_ = false) :
    Message(MSG_MDS_DISCOVER),
    base_ino(base_ino_),
    base_dir_frag(base_frag_),
    snapid(s),
    want(want_path_),
    want_ino(want_ino_),
    want_base_dir(want_base_dir_),
    want_xlocked(discover_xlocks_) { }
private:
  ~MDiscover() {}

public:
  const char *get_type_name() { return "Dis"; }
  void print(ostream &out) {
    out << "discover(" << header.tid << " " << base_ino << "." << base_dir_frag
	<< " " << want;
    if (want_ino)
      out << want_ino;
    out << ")";
  }

  void decode_payload(CephContext *cct) {
    bufferlist::iterator p = payload.begin();
    ::decode(base_ino, p);
    ::decode(base_dir_frag, p);
    ::decode(snapid, p);
    ::decode(want, p);
    ::decode(want_ino, p);
    ::decode(want_base_dir, p);
    ::decode(want_xlocked, p);
  }
  void encode_payload(CephContext *cct) {
    ::encode(base_ino, payload);
    ::encode(base_dir_frag, payload);
    ::encode(snapid, payload);
    ::encode(want, payload);
    ::encode(want_ino, payload);
    ::encode(want_base_dir, payload);
    ::encode(want_xlocked, payload);
  }

};

#endif
