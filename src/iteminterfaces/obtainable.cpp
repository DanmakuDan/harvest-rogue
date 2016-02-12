#include "..\..\include\iteminterfaces\obtainable.h"

Obtainable::Obtainable()
{
}

Obtainable::~Obtainable()
{
}

std::shared_ptr<Obtainable> Obtainable::Deserialize(picojson::value serializedValue)
{
   auto result = std::shared_ptr<Obtainable>(new Obtainable());

   if (!serializedValue.is<picojson::object>()) {
      throw;
   }

   auto data = serializedValue.get<picojson::object>();
   for (auto item : data) {

      if (item.first == "stackable") {
         if (!item.second.is<bool>()) {
            throw;
         }
         
         auto value = item.second.get<bool>();
         result->SetIsStackable(value);
         continue;
      }

      if (item.first == "maxStackSize") {
         if (!item.second.is<double>()) {
            throw;
         }

         auto value = item.second.get<double>();

         if (value != (unsigned int)value) {
            throw;
         }

         result->SetMaxStackSize((int)value);
         continue;
      }

      throw;
   }

   return result;
}

bool Obtainable::GetIsStackable()
{
   return this->Stackable;
}

void Obtainable::SetIsStackable(bool stackable)
{
   this->Stackable = stackable;
}

int Obtainable::GetMaxStackSize()
{
   return this->MaxStackSize;
}

void Obtainable::SetMaxStackSize(int maxStackSize)
{
   this->MaxStackSize = maxStackSize;
}

ItemInterfaceType::ItemInterfaceType Obtainable::GetInterfaceType()
{
   return ItemInterfaceType::Obtainable;
}
