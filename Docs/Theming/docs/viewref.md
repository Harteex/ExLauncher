# View Reference

## View (common)

Some attributes are applicable to all (or most) views.

#### width / height
Width and height are defined by either a value in pixels, * or **wrap_content**.
If * is specified, the view will fill the parent view. If multiple child views for a view specify *, the children will share the space.
The value wrap_content means the view will calculate its size from its child views.

#### id
Id is set so that ExLauncher can find some specific views. An example of an id value is **inputView**. ExLauncher will search for the inputView View and use this for key input.

#### name
Name of the view. Will be used by for example TabPanel to name its tabs.

#### layoutMargin
The margin the view should have against other views in the parent. Either a single value for all sides or the sides separated with a space: top bottom left right.

#### layoutGravity
The gravity the view should have within the parent. Multiple values can be used, separating them with a |. Acceptable values: **left**, **right**, **center_horizontal**, **top**, **bottom**, **center_vertical**. Example: left|bottom

#### gravity
The gravity the content of the view should have. Mostly used for content views, and more specifically, the Label. For values, see layoutGravity.

#### action
The action to be performed when using the primary action on a selected view in an input enabled parent view.
Current publically available action is to launch another screen, which can be done with the screen command:
`screen:<xml>[:args]`

Example 1:
`screen:@theme/myScreen.xml`

Example 2:
`screen:@theme/myScreen.xml:category=games`

#### tag
Tags are separated with |.

Tagging can be used to have ExLauncher fill a view with apps with the tag **fillItems**.

When filling items with tags, another special tag can be used to filter app categories. For example, to include apps from categories *applications* and *games*: `tag="fillItems|categories=applications,games"`

Categories can also be excluded. For example, to fill a view with apps from all categories except settings: `tag="fillItems|categories=-settings"`

#### background
Background defines the background color of a view. A color can be written as a hex code with alpha (#00000000), a hex code without alpha (#000000) or as a color name (black).
For acceptable color names, see a list of either CSS or .NET colors. Colors must be specified lowercase.

#### itemTemplate
ItemTemplate is mostly used with collection views. It specifies an XML file which contains a hierarchy of views defining how an item view in the current view should look like.
The template is used for both entries inserted by ExLauncher (when tagging up the view for filling with apps), or when the view **Item** is used.
This attribute can also be specified inline XAML style.


# Layouting Views

## LinearPanel

LinearPanel is one of the most important views for laying out elements.
It will place children next to each other, either vertically or horizontally.

#### orientation
Defines in which direction to lay out child views. Acceptable values: **horizontal**, **vertical**.

## FramePanel

FramePanel places its children on top of each other. The view has many purposes, one of which is to decorate one or many views with for example a border.

*No specific attributes*


# Collection Views

## ListView

#### orientation
Defines in which direction to lay out child views. Acceptable values: **horizontal**, **vertical**.

#### itemSize
ItemSize is a single value defining either row height or column width depending on orientation.

#### contentInset
Defines how far from the border a selected view should be. Leading and trailing values (in the set orientation) can be defined as two values with a space between them. If just a single value is provided, that value will apply for both leading and trailing content inset. If leading and trailing content inset overlap, the trailing value will be adjusted to be adjacent to the leading value, subtracted the size of the content view.

## GridView

#### orientation
Defines in which direction to lay out child views. Acceptable values: **horizontal**, **vertical**.

#### horizontalSpacing / verticalSpacing
These attributes control the spacing between elements in the grid.

#### itemWidth / itemHeight
The size of the elements in the grid.

#### contentInset
Defines how far from the border a selected view should be. Leading and trailing values (in the set orientation) can be defined as two values with a space between them. If just a single value is provided, that value will apply for both leading and trailing content inset. If leading and trailing content inset overlap, the trailing value will be adjusted to be adjacent to the leading value, subtracted the size of the content view.

## TabPanel

The TabPanel displays its children under each own tab. For the tabs' labels, the name attribute will be used.

*No specific attributes*


# Content Views

## Label

#### text
The text to be shown.

#### textSize
The size of the text.

#### textColor
Color of the text. For format, see the background attribute.

#### font
The font to use. Acceptable values: **regular**, **light**, **semibold**.

## Image

#### path
Path to the image including extension. For images within the theme, use @theme/ prefix to the path.

#### altPath
Alternative path to image, if the image for the specified path does not exist.

## RectangleShape

#### strokeColor
Color of the border of the rectangle. For format, see the background attribute.

#### fillColor
Fill color of the rectangle. For format, see the background attribute.

#### strokeWidth
The width of the border


# Other Views

## RecentAppView

#### recentIndex
A number representing which recent app should be shown. A value of 0 denotes the most recent app.

#### category
*Not yet in use*

## Selector

Selector hosts states, see State. Will be removed or remade in future versions.

*No specific attributes*

## State

Shows and hides child views depending on a state. **NOTE**: For this view you should not provide width/height or other attributes.

#### stateSelected
If the item is selected or not. Acceptable values: **true**, **false**

#### stateRecentType
Used to define the state of a RecentAppView. Acceptable value: **noItem**, **recentItem**. In future versions, **pinnedItem** will also be supported.

## Item
Will be replaced with the view defined by the itemTemplate.