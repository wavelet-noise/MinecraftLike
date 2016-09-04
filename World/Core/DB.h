#pragma once
#ifndef BlocksLibrary_h__
#define BlocksLibrary_h__

#include "GameObject.h"
#include <unordered_map>
#include "BlockTessellator.h"
#include <Core\Recipe.h>
#include <template\Template.h>

class DB
{
public:
	static DB &Get();

	void Registry(const StringIntern &name, PGameObject block, bool isStatic = true, bool isActive = false);
	void RegistryTesselator(const StringIntern &name, PGameObjectTessellator tess);
	void ReloadDirectory(const std::string &mDir);

	const std::vector<StringIntern> &Taglist(const StringIntern &tag) const;
	const std::vector<StringIntern> &Taglist(const std::string & tag) const;

	const std::vector<StringIntern> &Tag(const StringIntern &id) const;
	const std::vector<StringIntern> &Tag(const std::string & id) const;

	// Создать блок.
	// Если блок статический, возвращается указатель на экземпляр блока, хранящийся в библиотеке.
	// Если блок динамический, создается копия блока.
	// @param blockId Идентификатор блока.
	PGameObject Create(const std::string  & name) const;
	PGameObject Create(const StringIntern & name) const;

	PGameObjectTessellator CreateTesselator(const StringIntern &name) const;

	const std::list<PRecipe> &GetUsing(const std::string  & name) const;
	const std::list<PRecipe> &GetUsing(const StringIntern & name) const;

	const std::list<PRecipe> &GetRecipe(const std::string  & name) const;
	const std::list<PRecipe> &GetRecipe(const StringIntern & name) const;
	PDeepRecipe GetDeepRecipe(const StringIntern & name, const std::vector<StringIntern> &materials) const;

	const std::list<PRecipe> &GetMachine(const std::string  & name) const;
	const std::list<PRecipe> &GetMachine(const StringIntern & name) const;

	void AddRecipe(PRecipe r);

	std::unordered_map<StringIntern, std::tuple<PGameObject, bool, bool>> mObjects; //etalon, dynamic, active

private:
	friend class WindowDb;
	friend class Recipe;

	std::unordered_map<StringIntern, PGameObjectTessellator> mTess;
	std::unordered_map<StringIntern, std::vector<StringIntern>> mTags; // what, where
	std::unordered_map<StringIntern, std::vector<StringIntern>> mObjTags; // where, what
	std::list<std::shared_ptr<Template>> mTempl;
	std::vector<StringIntern> mTagsEmptyDummy;

	std::list<PRecipe> mRecipe;
	std::unordered_map<StringIntern, std::list<PRecipe>> mUsingCache;
	std::unordered_map<StringIntern, std::list<PRecipe>> mRecipeCache;
	std::unordered_map<StringIntern, std::list<PRecipe>> mMachineCache;

private:
	DB() {};
	~DB() {};
	DB(const DB &) = delete;
	DB& operator=(const DB &) = delete;

};



#endif // BlocksLibrary_h__