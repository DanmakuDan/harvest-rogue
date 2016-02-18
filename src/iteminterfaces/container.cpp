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
   return this->Contents;
}

ItemContainerPtr Container::AsItemContainer()
{
   return static_cast<ItemContainerPtr>(this);
}

void Container::AddItem(ItemPtr item, int count, bool dontStack)
{
   if (!dontStack) {
      throw; // This is not implemented yet.
   }

   auto newItem = std::make_shared<Item>(Item::Clone(*item));
   this->Contents.push_back(newItem);
}

void Container::RemoveItem(ItemPtr item, int count)
{
   auto itemLocation = std::find(this->Contents.begin(), this->Contents.end(), item);
   if (itemLocation == this->Contents.end()) {
      return;
   }
   this->Contents.erase(itemLocation);
}

void Container::SwapItem(ItemPtr itemA, ItemPtr itemB)
{
   auto firstItemLocation = std::find(this->Contents.begin(), this->Contents.end(), itemA);
   if (firstItemLocation == this->Contents.end()) {
      return;
   }

   auto secondItemLocation = std::find(this->Contents.begin(), this->Contents.end(), itemB);
   if (secondItemLocation == this->Contents.end()) {
      // We're actually swapping in an item that we don't already have
      firstItemLocation->swap(itemB);
      return;
   }
   // We're swapping between two items we have
   std::iter_swap(firstItemLocation, secondItemLocation);
}

void Container::SplitItem(ItemPtr item)
{
   auto itemLocation = std::find(this->Contents.begin(), this->Contents.end(), item);
   if (itemLocation == this->Contents.end()) {
      return;
   }

   auto newItem = std::make_shared<Item>(Item::Clone(*item));
   auto splitSize = item->GetCount() / 2;
   auto difference = (item->GetCount() - splitSize); // We don't want rounding issues
   item->SetCount(splitSize);
   newItem->SetCount(difference);
   this->Contents.insert(itemLocation + 1, newItem);
}

void Container::CombineItems(ItemPtr source, ItemPtr dest)
{
   auto destItemLocation = std::find(this->Contents.begin(), this->Contents.end(), dest);
   auto sourceItemLocation = std::find(this->Contents.begin(), this->Contents.end(), source);

   auto movingFromHere = sourceItemLocation != this->Contents.end();
   auto movingToHere = destItemLocation != this->Contents.end();

   if (!movingFromHere && !movingToHere) {
      return;
   }

   if (movingToHere) {
      destItemLocation->get()->SetCount(destItemLocation->get()->GetCount() + source->GetCount());
      source->SetCount(0);
   }

   if (!movingFromHere) {
      return;
   }

   this->Contents.erase(sourceItemLocation);
}

std::string Container::GetName()
{
   return "Container";
}
