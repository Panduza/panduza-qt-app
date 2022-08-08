#ifndef NODEFAMILYARITHMETIC_HPP
#define NODEFAMILYARITHMETIC_HPP

#include <NodeOperation.hpp>
#include <NodeEntryDouble.hpp>
#include <NodePropertyDropdown.hpp>

class N_Arithmetic_Math : public NodeOperation
{
    public:
        N_Arithmetic_Math(NoderScene *scene)
            : NodeOperation(scene, "Math")
        {
            QStringList menu_list;
            
            value = addNodeEntryOutput<NodeEntryDouble>("Value");
            a = addNodeEntryInput<NodeEntryDouble>("A");
            b = addNodeEntryInput<NodeEntryDouble>("B");
        
            _operlist["Add"] = [&]{return (a->value() + b->value());};
            _operlist["Substract"] = [&]{return (a->value() - b->value());};
            _operlist["Multiply"] = [&]{return (a->value() * b->value());};
            _operlist["Divide"] = [&]{return (a->value() / b->value());};
            _operlist["Modulo"] = [&]{return (std::fmod(a->value(), b->value()));};
            _operlist["Power"] = [&]{return (std::pow(a->value(), b->value()));};
        
            for (auto op: _operlist) {menu_list.push_front(op.first);}
        
            menu = addNodeProperty<NodePropertyDropdown>();
            menu->setDropdownList(menu_list);
        }
        ~N_Arithmetic_Math() = default;

        bool exec(void) override
        {
            value->sendValue(_operlist[menu->value()]());
            return true;
        }

    private:
        std::unordered_map<QString, std::function<double()>> _operlist;
        NodeEntryDouble *a, *b;
        NodeEntryDouble *value;
        NodePropertyDropdown *menu;
};
#endif