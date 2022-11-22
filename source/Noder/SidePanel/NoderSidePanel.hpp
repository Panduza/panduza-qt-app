#pragma once

#include <QVBoxLayout>
#include <QFile>

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
#define DEFAULT_VARIABLE_TYPE       NoderVar::Type::Bool
#define DEFAULT_VARIABLE_CONTAINER  NoderVar::Container::Variable
#define DEFAULT_PIN_DIRECTION       PinProperty::Direction::Input
#define DEFAULT_PIN_CONTAINER       NoderVar::Container::Variable
#define DEFAULT_PIN_TYPE            NoderVar::Type::Bool

template <class N>
class NoderSPEntry : public PzaWidget
{
    public:
        virtual void setName(const QString &name)
        {
            _name = name;
            _nameEntry->setText(name);
            //_elem->setName(name);
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
            : PzaWidget(parent)
        {
            _layout = new QHBoxLayout(this);
            _deleteBtn = new PzaPushButton(this);
            _propBtn = new PzaPushButton(this);
            _nameEntry = new PzaNameEntry(this);
            _elem = new N();

            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

            connect(_nameEntry, &PzaNameEntry::newNameRequested, this, [&](const QString &s) {
            //    bool ok = true;
            //    auto list = Noder::Get().Frame->SidePanel.VariableArea->entryList();
            //    for (auto const &item : list) {
            //        if (item->name() == s) {
            //            ok = false;
            //            break;
            //        }
            //    }
            //    (ok) ? setName(s) : setName(name());
            });

            connect(_deleteBtn, &PzaPushButton::clicked, [&](){remove();});

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
class NoderSPArea : public PzaWidget
{
    public:
        std::vector<N *> &entryList(void) {return _entryList;}

    protected:
        NoderSPArea<N>(QWidget *parent)
            : PzaWidget(parent)
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

            const QString &name = PzaUtils::allocateName<N *>(_entryList, _defaultEntryName, getNameInVector);

            entry = new N(this);
            entry->setName(name);

            connect(entry, &N::removed, this, [&, entry](){
                removeEntry(entry);
            });
            connect(entry, &N::clicked, this, [&, entry]() {
                selectEntry(entry);
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

                if (index < _entryList.size())
                    next = _entryList.at(index);
                else if (index < 2)
                    next = nullptr;
                else if (_entryList.size() > 0)
                    next = _entryList.at(index - 2);

                selectEntry(next);
            }

            PzaUtils::DeleteFromVector(_entryList, target);
            target->elem()->deleteLater();
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

        void save();

    private:
        NoderSP(QWidget *parent = nullptr);
        
        PzaWidget *_main;
        QVBoxLayout *_layout;
};