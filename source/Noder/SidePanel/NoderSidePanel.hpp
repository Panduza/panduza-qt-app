#pragma once

#include <QVBoxLayout>
#include <QFile>
#include <QDrag>

#include <PzaWidget.hpp>
#include <PzaSpoiler.hpp>
#include <PzaScrollArea.hpp>
#include <PzaPushButton.hpp>
#include <PzaNameEntry.hpp>
#include <PzaUtils.hpp>
#include "Noder.hpp"

class NoderSPVarArea;
class NoderSPFunctionArea;

#define DEFAULT_FUNCTION_NAME       "New Function"
#define DEFAULT_PIN_NAME            "New Pin"
#define DEFAULT_VARIABLE_NAME       "New Variable"
#define DEFAULT_VARIABLE_VAR(x)     x = {NoderVarProps::Container::Reference, NoderVarProps::Type::Bool, ""}
#define DEFAULT_PIN_DIRECTION       PinProperty::Direction::Input

class NoderSPEntryAbstract : public PzaWidget
{
    Q_OBJECT

    public:
        NoderSPEntryAbstract(QWidget *parent = nullptr) : PzaWidget(parent) {}

    signals:
        void requestedNewName(const QString &name);

};

template <class N>
class NoderSPEntry : public NoderSPEntryAbstract
{
    public:
        virtual void setName(const QString &name)
        {
            _name = name;
            _nameEntry->setText(name);
            if (_elem)
                _elem->setName(name);
        }

        const QString &name(void) {return _name;}

        virtual void setSelected(bool state)
        {
            if (state == true) {
                setStyleSheet("background-color: #1B2426");
            }
            else
                setStyleSheet("background-color: transparent");
        }

        void setElem(N *elem) {_elem = elem;}
        N *elem(void) const {return _elem;}

    protected:
        NoderSPEntry<N>(QWidget *parent = nullptr)
            : NoderSPEntryAbstract(parent),
            _elem(nullptr)
        {
            _layout = new QHBoxLayout(this);
            _deleteBtn = new PzaPushButton(this);
            _propBtn = new PzaPushButton(this);
            _nameEntry = new PzaNameEntry(this);

            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

            connect(_nameEntry, &PzaNameEntry::newNameRequested, this, [&](const QString &s) { requestedNewName(s); });
            connect(_deleteBtn, &PzaPushButton::clicked, [&]() { remove(); });

            _layout->setContentsMargins(0, 0, 0, 0);
            _layout->setSpacing(5);

            _propBtn->setFixedSize(20, 20);
            _deleteBtn->setFixedSize(20, 20);

            _layout->addWidget(_nameEntry);
            _layout->addWidget(_propBtn);
            _layout->addWidget(_deleteBtn);
        }

        virtual void remove(void) = 0;

        N *_elem;
        QString _name;
        QHBoxLayout *_layout;
        PzaPushButton *_propBtn;
        PzaPushButton *_deleteBtn;
        PzaNameEntry *_nameEntry;
};

template <class N>
class NoderSPArea : public QWidget
{
    public:
        std::vector<N *> &entryList(void) {return _entryList;}

        N *selected(void) const {return _selectedEntry;}

    protected:
        NoderSPArea<N>(QWidget *parent = nullptr)
            : QWidget(parent)
        {
            _layout = new QVBoxLayout(this);
            _entryTable = new PzaWidget(this);
            _entryTableLayout = new QVBoxLayout(_entryTable);
            _addBtn = new PzaPushButton(this);

            connect(_addBtn, &PzaPushButton::clicked, this, &NoderSPArea::newEntry);

            _layout->addWidget(_addBtn);
            _layout->addWidget(_entryTable);
        }

        virtual N *newEntry(void)
        {
            N *entry;

            auto getNameInVector = [](N *entry) -> const QString & {
                return entry->name();
            };

            const QString &name = PzaUtils::AllocateName<N *>(_entryList, _defaultEntryName, getNameInVector);

            entry = new N(this);
            entry->setName(name);

            connect(entry, &N::removed, this, [&, entry]() {
                removeEntry(entry);
            });
            connect(entry, &N::clicked, this, [&, entry]() {
                selectEntry(entry);
            });
            connect(entry, &N::requestedNewName, this, [&, entry](const QString &s) {
                bool ok = true;
                for (auto const &item : _entryList) {
                    if (item->name() == s) {
                        ok = false;
                        break;
                    }
                }
                (ok) ? entry->setName(s) : entry->setName(entry->name());
            });

            _entryList.push_back(entry);
            _entryTableLayout->addWidget(entry);

            return entry;
        }

        virtual void removeEntry(N *target)
        {
            if (_selectedEntry == target) {
                N *next = nullptr;
                size_t index = PzaUtils::IndexInVector<N *>(_entryList, _selectedEntry) + 1;

                if (_entryList.size() < 2)
                    next = nullptr;
                else if (index < _entryList.size())
                    next = _entryList.at(index);
                else
                    next = _entryList.at(index - 2);

                selectEntry(next);
            }

            PzaUtils::DeleteFromVector(_entryList, target);
            target->deleteLater();
        }

        virtual void selectEntry(N *target)
        {
            if (_selectedEntry)
                _selectedEntry->setSelected(false);
            if (target) {
                target->setSelected(true);
            }
            _selectedEntry = target;
        }

        virtual void removeAllEntries(void) {}

        PzaPushButton *_addBtn;
        QVBoxLayout *_layout;
        PzaWidget *_entryTable;
        QVBoxLayout *_entryTableLayout;
        std::vector<N *> _entryList;
        N *_selectedEntry = nullptr;
        QString _defaultEntryName;
};

class NoderSP : public PzaScrollArea
{
    Q_OBJECT

    friend class NoderFrame;

    public:
        NoderSP(NoderSP &other) = delete;
        void operator=(const NoderSP &) = delete;

        NoderSPVarArea *VariableArea = nullptr;
        NoderSPFunctionArea *FunctionArea = nullptr;
        PzaSpoiler *DefValArea = nullptr;

        void save();

    private:
        NoderSP(QWidget *parent = nullptr);
        
        PzaWidget *_main;
        QVBoxLayout *_layout;
};