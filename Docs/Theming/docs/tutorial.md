# Tutorial

### Creating the theme files

Create a new folder in the themes directory. The name of the folder is your theme's ID and it's recommended to name it in lowercase using only a-z and 0-9.

Inside the new folder, create the file theme.xml. Use the following content as a starting point and edit as you wish.

	<?xml version="1.0" encoding="utf-8"?>
	<Theme>
	  <Name>My Theme</Name>
	  <EntryPoint>start.xml</EntryPoint>
	  <Author>The Author</Author>
	  <Copyright>Copyright (c) The Author year</Copyright>
	  <License>CC Attribution 4.0</License>
	  <ThirdPartyDeclarations>
		<ContentDeclaration>Any credits or license for third party content</ContentDeclaration>
		<ContentDeclaration>Multiple content declaration can be used</ContentDeclaration>
	  </ThirdPartyDeclarations>
	</Theme>

Now, for the theme to actually work, we need to have an entry point. You define the name of you entry point in theme.xml.

In this tutorial I will go with start.xml. Create your entry point xml and use the following as your entry point.

	<?xml version="1.0" encoding="utf-8"?>
	<LinearPanel
	  xmlns="http://schemas.exlauncher.com/exlauncher"
	  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	  xsi:schemaLocation="http://schemas.exlauncher.com/exlauncher ../../schema/screen.xsd"
	  width="*"
	  height="*">

	</LinearPanel>

![Image](images/1-empty-screen.png)

Now start ExLauncher with the command line argument `--theme=yourthemeid`. You should see a blank screen showing only a background image.


### Adding views

Now it's time to actually build theme.

Let's start by adding a text label to the LinearPanel. The view name for a text label is `Label`.

	<?xml version="1.0" encoding="utf-8"?>
	<LinearPanel
	  xmlns="http://schemas.exlauncher.com/exlauncher"
	  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	  xsi:schemaLocation="http://schemas.exlauncher.com/exlauncher ../../schema/screen.xsd"
	  width="*"
	  height="*">

	  <Label
		width="wrap_content"
		height="wrap_content"
		text="My awesome theme"
		textSize="24"
		textColor="white"
		font="regular" />

	</LinearPanel>

![Image](images/2-adding-views.png)

You may notice that the linear panel and the label use different values for width and height. The linear panel uses a star, which means it will size itself so that it fills its parent. As the linear panel already is highest in the hierarchy, it will fill the screen.

The label instead uses **wrap_content**, which means it will have the size of its content. In other words, the size will be just large enough to display the text in it.

Next up we want to show categories of apps for the user to select. For this tutorial we will use a simple list for this purpose.

The list view also requires another attribute, **itemSize**. If the orientation of the list is vertical, itemSize is the row height, and if the orientation is horizontal, it's the column width. One more change required is to set the LinearPanel orientation to vertical.

	<?xml version="1.0" encoding="utf-8"?>
	<LinearPanel
	  xmlns="http://schemas.exlauncher.com/exlauncher"
	  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	  xsi:schemaLocation="http://schemas.exlauncher.com/exlauncher ../../schema/screen.xsd"
	  width="*"
	  height="*"
	  orientation="vertical">

	  <Label
		width="wrap_content"
		height="wrap_content"
		text="My awesome theme"
		textSize="24"
		textColor="white"
		font="regular" />

	  <ListView
		width="*"
		height="*"
		orientation="vertical"
		itemSize="40" />

	</LinearPanel>

When you run your theme it will look exactly the same as before. This is because we don't have anything in the list yet.

If you want to check and see that the list view is actually there, you can see outlines of views by launching ExLauncher with `--debugViewBounds`.

![Image](images/3-debug-view-bounds.png)


### The itemTemplate

To add categories we could just hard code 4 list items; games, emulators, applications and settings. However, this is a bad idea, since there could exist more categories we don't know about.
So what we're going to do is tell ExLauncher to fill our list for us.

But when filling, ExLauncher needs to know how we want each list entry to look like, and that's what itemTemplate is there for.

TODO

### Filling the list

TODO

### The app screen

TODO