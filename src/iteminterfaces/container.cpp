/*
    harvest-rogue is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    harvest-rogue is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.     */

#include "container.h"
#include "gamestate.h"
#include "itemtransferdialog.h"
#include "player.h"

Container::Container()
{
}

Container::~Container()
{
}

std::shared_ptr<Container> Container::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Container>(new Container());

   return result;
}

ItemInterfaceType::ItemInterfaceType Container::GetInterfaceType()
{
   return ItemInterfaceType::Container;
}

void Container::Interact(ItemPtr sourceItem)
{
   GameState::Get().PushDialog(ItemTransferDialog::Construct(Player::Get().AsItemContainer(), this->AsItemContainer()));
}

ItemListPtr Container::GetAllItems()
{
   return this->GetAllItems();
}

ItemContainerPtr Container::AsItemContainer()
{
   return static_cast<ItemContainerPtr>(this);
}

std::string Container::GetName()
{
   return "Container";
}