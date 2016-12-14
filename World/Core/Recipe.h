#pragma once

#include <vector>
#include <tools\StringIntern.h>
#include <rapidjson\document.h>
#include <list>

class RecipeIn
{
public:
	StringIntern id;
	int count = 1;

	// <"id", [count]>
	virtual void JsonLoad(const rapidjson::Value &val);

	bool operator == (const RecipeIn &other) const
	{
		if (&other == this)
			return true;

		return id == other.id && count == other.count;
	}
};

class RecipeOut
{
public:
	StringIntern id;
	int count = 1;
	float chance = 1.0;

	// <"id", [count[, chance]]>
	virtual void JsonLoad(const rapidjson::Value &val);

	bool operator == (const RecipeOut &other) const
	{
		if (&other == this)
			return true;

		return id == other.id && count == other.count && chance == other.chance;
	}
};

class Recipe;
class Chest;

using PRecipe = std::shared_ptr<Recipe>;
using PCRecipe = std::shared_ptr<const Recipe>;

using PDeepRecipe = std::shared_ptr <class DeepRecipe>;

class Recipe : public std::enable_shared_from_this<Recipe>
{
public:
	virtual ~Recipe(){}

	std::vector<RecipeIn> input;
	std::vector<RecipeOut> output;
	std::vector<StringIntern> tools;
	StringIntern machine;
	std::vector<StringIntern> materials;

	float duration = 0;

	bool DrawGui(float gt, bool &);

	std::list<std::shared_ptr<Recipe>> Expand();

	virtual void JsonLoad(const rapidjson::Value &val);

	bool CraftIn(Chest &c, int count = 1);

	bool operator == (const Recipe &other) const
	{
		if (&other == this)
			return true; 

		//really there is no need to comparison
		//recipes exists only in one instance

		//if (input.size() != other.input.size())
		//	return false;

		//if (output.size() != other.output.size())
		//	return false;

		//if (tools.size() != other.tools.size())
		//	return false;

		//for (int i = 0; i < input.size(); ++i)
		//	if (!(input[i] == other.input[i]))
		//		return false;

		//for (int i = 0; i < output.size(); ++i)
		//	if (!(output[i] == other.output[i]))
		//		return false;

		//for (int i = 0; i < tools.size(); ++i)
		//	if (!(tools[i] == other.tools[i]))
		//		return false;

		//if (duration != other.duration)
		//	return false;

		//return machine == other.machine;

		return false;
	}
};

class DeepRecipe
{
public:
	PRecipe recipe;
	std::vector<PDeepRecipe> input_expantion;
	bool incomplete = false;
	float need_count = 1;
	bool first_show = true;

	bool DrawGui(float gt);
};