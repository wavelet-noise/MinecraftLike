// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef BlocksLibrary_h__
#define BlocksLibrary_h__

#include "GameObject.h"
#include <unordered_map>
#include "BlockTessellator.h"

class DB
{
public:
  static DB &Get();

  void Registry(const StringIntern &name, PGameObject block, bool isStatic = true);
  void ReloadDirectory(const std::string &dir);
  const std::vector<StringIntern> &Taglist(const StringIntern &name);

  void PushModel(const StringIntern &s, PModel &m);
  PModel GetModel(const StringIntern &s);

  /// ������� ����.
  /// ���� ���� �����������, ������������ ��������� �� ��������� �����, ���������� � ����������.
  /// ���� ���� ������������, ��������� ����� �����.
  /// @param blockId ������������� �����.
  PGameObject Create(const std::string & name);
  PGameObject Create(const StringIntern &name);

  PGameObjectTessellator CreateTesselator(const StringIntern &name);

private:
  friend class WindowDb;
  std::unordered_map<StringIntern, PGameObject> mObjects;
  std::unordered_map<StringIntern, PGameObjectTessellator> mTess;
  std::unordered_map<StringIntern, PModel> mModel;
  std::unordered_map<StringIntern, std::vector<StringIntern>> mTags;

private:
  DB() {};
  ~DB() {};
  DB(const DB &) = delete;
  DB& operator=(const DB &) = delete;

};



#endif // BlocksLibrary_h__