#ifndef MaterialDictionary_H
#define MaterialDictionary_H
#include "Agent.h"
#include <vector>

class MaterialDictionary : public Agent
{

public:
    // Унаследовано через Agent
    virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
    virtual void Update(const GameObjectParams & params) override;
    virtual void Afterload(GameObject * parent) override;
    MaterialDictionary();
    void JsonLoad(const rapidjson::Value &val) override;

    std::vector<StringIntern> materials;
};

REGISTER_AGENT(MaterialDictionary)

#endif // MaterialDictionary_H
