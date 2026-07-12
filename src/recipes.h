#ifndef RECIPES_H
#define RECIPES_H

/* Print a recipe for an item using recipes.json */
void print_recipe(const char *item, const char *recipes_path);
void list_items(const char *recipes_path);
int write_item_list(const char *recipes_path, const char *list_path);

#endif /* RECIPES_H */
