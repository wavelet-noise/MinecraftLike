#ifndef TAGS_H
#define TAGS_H
#include "Agent.h"
#include <vector>

class Tags : public Agent
{

public:
    std::vector<StringIntern> tags;

    // Унаследовано через Agent
    virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
    virtual void Update(const GameObjectParams & params) override;
    Tags();
    void JsonLoad(const rapidjson::Value &val) override;
};

REGISTER_AGENT(Tags)

#endif // TAGS_H
