
////    HTREEITEM hPrev = (HTREEITEM)TVI_FIRST; 
////    HTREEITEM hPrevRootItem = NULL; 
////    HTREEITEM hPrevLev2Item = NULL; 
//
////
//static HTREEITEM addTreeItem(HWND hTree, LPTSTR lpszItem, HTREEITEM hParent, HTREEITEM hPrev, TVImages tvImages) { 
//    TVITEM			tvi		= {}; 
//    tvi.mask				= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
//    tvi.pszText			= lpszItem; 
//    tvi.cchTextMax		= sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
//    tvi.iImage			= tvImages.Material; 
//    tvi.iSelectedImage	= tvImages.Material; 
//    tvi.lParam			= (LPARAM)nLevel; 
//
//    TVINSERTSTRUCT	tvins	= {}; 
//    tvins.item			= tvi; 
//    tvins.hInsertAfter	= hPrev; 
//    tvins.hParent			= hParent; 
//
//    // Add the item to the tree-view control. 
//    HTREEITEM hItem = (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 
//                                                                                                                                                
//    if (hItem == NULL)
//        return NULL;
//
//    // The new item is a child item. Give the parent item a closed folder bitmap to indicate it now has child items. 
//    if (nLevel > 1) { 
//		HTREEITEM			hti		= TreeView_GetParent(hTree, hPrev); 
//        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
//        tvi.hItem = hti; 
//        tvi.iImage = g_nClosed; 
//        tvi.iSelectedImage = g_nClosed; 
//        TreeView_SetItem(hTree, &tvi); 
//    } 
//
//    return hPrev; 
//} 
////
////// Extracts heading text and heading levels from a global array and passes them to a function that adds them as parent and child items to a tree-view control. 
////// Returns TRUE if successful, or FALSE otherwise. hTree - handle to the tree-view control. 
////
////BOOL InitTreeViewItems(HWND hTree)
////{ 
////    HTREEITEM hti;
////
////    // g_rgDocHeadings is an application-defined global array of 
////    // the following structures: 
////    //     typedef struct 
////    //       { 
////    //         TCHAR tchHeading[MAX_HEADING_LEN]; 
////    //         int tchLevel; 
////    //     } Heading; 
////    for (int i = 0; i < ARRAYSIZE(g_rgDocHeadings); i++) 
////    { 
////        // Add the item to the tree-view control. 
////        hti = AddItemToTree(hTree, g_rgDocHeadings[i].tchHeading, 
////            g_rgDocHeadings[i].tchLevel); 
////
////        if (hti == NULL)
////            return FALSE;
////    } 
////           
////    return TRUE; 
////}
//// constexpr TCHAR ICON_SUM = TEXT("%systemroot%\system32\wmploc.dll");