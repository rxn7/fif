#pragma once

struct TagComponent {
	TagComponent(const std::string &tag) : tag(tag) {}
	TagComponent(const char *tag) : tag(tag) {}

	std::string tag;
};
