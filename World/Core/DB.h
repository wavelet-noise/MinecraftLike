#pragma once
#ifndef BlocksLibrary_h__
#define BlocksLibrary_h__

#include "GameObject.h"
#include <unordered_map>
#include "BlockTessellator.h"

class Template;

class DB
{
public:
  static DB &Get();

  void Registry(const StringIntern &name, PGameObject block, bool isStatic = true);
  void ReloadDirectory(const std::string &mDir);
  const std::vector<StringIntern> &Taglist(const StringIntern &name);

  // Создать блок.
  // Если блок статический, возвращается указатель на экземпляр блока, хранящийся в библиотеке.
  // Если блок динамический, создается копия блока.
  // @param blockId Идентификатор блока.
  PGameObject Create(const std::string  & name);
  PGameObject Create(const StringIntern & name);

  PGameObjectTessellator CreateTesselator(const StringIntern &name);

private:
  friend class WindowDb;
  std::unordered_map<StringIntern, std::tuple<PGameObject, bool>> mObjects;
  std::unordered_map<StringIntern, PGameObjectTessellator> mTess;
  std::unordered_map<StringIntern, std::vector<StringIntern>> mTags;
  std::list<std::shared_ptr<Template>> mTempl;

private:
  DB() {};
  ~DB() {};
  DB(const DB &) = delete;
  DB& operator=(const DB &) = delete;

};



#endif // BlocksLibrary_h__