#include "pch.h"
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "ScopeHandler.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(ParseCoordinator::Wrapper);

void ParseCoordinator::Wrapper::IncrementDepth()
{
	_depth++;
}

void ParseCoordinator::Wrapper::DecrementDepth()
{
	_depth--;
}

ParseCoordinator::ParseCoordinator(Wrapper& wrapper) : currWrapper(&wrapper)
{

}

size_t ParseCoordinator::AddHandler(unique_ptr<IParseHandler>&& handler)
{
	size_t currRef = _nextRef;
	_handlers[_nextRef++] = std::move(handler);

	return currRef;
}

void ParseCoordinator::RemoveHelper(IParseHandler& handler)
{

}

bool ParseCoordinator::DeserializeObject(std::string json)
{
	Json::Value root;
	Json::CharReaderBuilder builder;
	JSONCPP_STRING err;

	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

	if (!reader->parse(json.c_str(), json.c_str() + json.length(), &root, &err))
	{
		throw std::logic_error(err.c_str());
		return false;
	}

	//int val = root["int"].asInt();

	return ParseMembers(root);
}

bool ParseCoordinator::DeserialzeObjectFromFile(std::string filename)
{
	return false;
}

bool ParseCoordinator::DeserializeObject(std::istream object)
{
	//if (!reader->parse(data.c_str(), data.c_str() + dataLength, &root, &err))
	return false;
}

bool ParseCoordinator::ParseMembers(Json::Value obj)
{
	bool result = true;
	//currWrapper->IncrementDepth();
	for (auto it = obj.begin(); it != obj.end(); ++it)
	{
		result = Parse(it.key().asString(), *it);
		if (!result)
		{
			break;
		}
	}

	return result;
}

bool ParseCoordinator::Parse(std::string key, Json::Value obj)
{
	for (auto it = _handlers.begin(); it != _handlers.end(); ++it)
	{
		if (obj.isObject())	// if we hit a nested scope
		{
			// make a Datum in our Scope* that holds another Scope*
			// current scope pointer now points at the child scope
			if (it->second->StartHandler(key, obj, currWrapper))	
			{
				currWrapper->IncrementDepth();
				ParseMembers(obj);	// parse this scope's members first

				ScopeHandler::ScopeWrapper* scopeWrapper = currWrapper->As<ScopeHandler::ScopeWrapper>();	// this should always succeed if given the right wrapper
				if (scopeWrapper == nullptr) { throw std::logic_error("Not using ScopeWrapper!"); }

				scopeWrapper->SetCurrentScopeToParent();	// go back up to our parent scope and point at it (feel like this shouldnt be here...)
				currWrapper->DecrementDepth();	// we finished Prasing members, reset depth
				return true;
			}
		}
		else if (it->second->StartHandler(key, obj, currWrapper))
		{
			it->second->EndHandler(key, currWrapper);
			return true;
		}
	}
	return false;
}