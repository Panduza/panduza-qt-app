#ifndef NODEFAMILYIO_HPP
#define NODEFAMILYIO_HPP

#include <NodeExec.hpp>
#include <NodeEntryInterface.hpp>
#include <NodePropertyDropdown.hpp>
#include <Io.hpp>

class N_Io_Common
{
    public:
        static const QList<QString> &directionList(void)
        {
            static QList<QString> _list = {"Input", "Output"};
            return _list;
        }

        static const QList<QString> &valueList(void)
        {
            static QList<QString> _list = {"Low", "High"};
            return _list;
        }

        static const QString direction(const QString &drop)
        {
            return (drop == "Input") ? "in" : "out";
        }

        static const int value(const QString &drop)
        {
            return (drop == "Low") ? 0 : 0;
        }

        static void sendDirection(Io *io, const QString &drop)
        {
            QString direction = N_Io_Common::direction(drop);
            io->sendDirection(direction);
        }

        static void sendValue(Io *io, const QString &drop)
        {
            int value = N_Io_Common::value(drop);
            io->sendValue(value);
        }
    
    private:
        N_Io_Common() = delete;
        ~N_Io_Common() = delete;
};

class N_Io_Init : public NodeExec
{
    public:
        N_Io_Init(NoderScene *scene)
            : NodeExec(scene, "I/O Initialize")
        {
            _interface = addNodeEntryInput<NodeEntryInterface>("Target");
            _direction = addNodeProperty<NodePropertyDropdown>("Direction");
            _value = addNodeProperty<NodePropertyDropdown>("Value");

            _direction->setDropdownList(N_Io_Common::directionList());
            _value->setDropdownList(N_Io_Common::valueList());
        }
        ~N_Io_Init() = default;

        bool exec(void) override
        {
            Io *io;

            io = dynamic_cast<Io *>(_interface->object());
            if (!io)
                return false;

            N_Io_Common::sendDirection(io, _direction->value());
            N_Io_Common::sendValue(io, _value->value());

            return true;
        }
    private:
        NodeEntryInterface *_interface;
        NodePropertyDropdown *_direction;
        NodePropertyDropdown *_value;
};

class N_Io_Value : public NodeExec
{
    public:
        N_Io_Value(NoderScene *scene)
            : NodeExec(scene, "I/O Set Value")
        {
            _interface = addNodeEntryInput<NodeEntryInterface>("Target");
            _value = addNodeProperty<NodePropertyDropdown>("Value");
            _value->setDropdownList(N_Io_Common::valueList());
        }
        ~N_Io_Value() = default;

        bool exec(void) override
        {
            Io *io;

            io = dynamic_cast<Io *>(_interface->object());
            if (!io)
                return false;

            qDebug() << "Set Value" << io->name();
            return true;
        }
    private:
        NodeEntryInterface *_interface;
        NodePropertyDropdown *_value;
};

class N_Io_Direction : public NodeExec
{
    public:
        N_Io_Direction(NoderScene *scene)
            : NodeExec(scene, "I/O Set Direction")
        {
            _interface = addNodeEntryInput<NodeEntryInterface>("Target");
            _direction = addNodeProperty<NodePropertyDropdown>("Direction");
            _direction->setDropdownList(N_Io_Common::directionList());
        }
        ~N_Io_Direction() = default;

        bool exec(void) override
        {
            Io *io;

            io = dynamic_cast<Io *>(_interface->object());
            if (!io)
                return false;

            qDebug() << "Set Direction" << io->name();
            return true;
        }
    private:
        NodeEntryInterface *_interface;
        NodePropertyDropdown *_direction;
};


#endif