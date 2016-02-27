#ifndef METAITEM_H
#define METAITEM_H
#include "Agent.h"

class Metaitem : public Agent
{

public:
    // Унаследовано через Agent
    virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
    virtual void Update(const GameObjectParams & params) override;
    Metaitem();
    void JsonLoad(const rapidjson::Value &val) override;
};

REGISTER_AGENT(Metaitem)

#endif // METAITEM_H
