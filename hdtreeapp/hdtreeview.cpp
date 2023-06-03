//#include "hdtreeview.h"
//#include <CommCtrl.h>
//
//constexpr int ID_TREEVIEW = 101;
//
//constexpr int CX_BITMAP     = 16;
//constexpr int CY_BITMAP     = 16;
//constexpr int NUM_BITMAPS   = 1;
//
//struct TVImages {
//    int CategoryEmpty   = 0;
//    int CategoryOpen    = 0;
//    int CategoryClosed  = 0;
//    int Material        = 0;
//};
//
//bool imageAdd(HINSTANCE hInstance, HIMAGELIST himl, const TCHAR* filename, int & out_imageIndex) {
//    HBITMAP hbmp = LoadBitmap(hInstance, filename); 
//    out_imageIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL); 
//    DeleteObject(hbmp); 
//    return out_imageIndex >= 0;
//}
//
//static bool InitTreeViewImageLists(HINSTANCE hInstance, HWND hwndTV, TVImages & out_tvImages) { 
//    HIMAGELIST himl; // handle to image list 
//    // Create the image list. 
//    if ((himl = ImageList_Create(CX_BITMAP, CY_BITMAP, FALSE, NUM_BITMAPS, 0)) == NULL) 
//        return false; 
//
//    if(imageAdd(hInstance, himl, TEXT("CategoryEmpty.bmp"), out_tvImages.CategoryEmpty)) 
//        return false;
//    if(imageAdd(hInstance, himl, TEXT("CategoryOpen.bmp"), out_tvImages.CategoryOpen)) 
//        return false;
//    if(imageAdd(hInstance, himl, TEXT("CategoryClosed.bmp"), out_tvImages.CategoryClosed)) 
//        return false;
//    if(imageAdd(hInstance, himl, TEXT("Material.bmp"), out_tvImages.Material)) 
//        return false;
//    
//    if (ImageList_GetImageCount(himl) < 4) // Fail if not all of the images were added. 
//        return false; 
//
//    TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL); // Associate the image list with the tree-view control. 
//    return true;
//} 
//
//HTREEITEM AddMaterial(HWND hwndTV, LPTSTR lpszItem, int nLevel, TVImages tvImages) { 
//    static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST; 
//    static HTREEITEM hPrevRootItem = NULL; 
//    static HTREEITEM hPrevLev2Item = NULL; 
//    HTREEITEM hti; 
//
//    TVINSERTSTRUCT tvins; 
//    TVITEM tvi; 
//    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
//    tvi.pszText = lpszItem; 
//    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
//    tvi.iImage = tvImages.Material; 
//    tvi.iSelectedImage = tvImages.Material; 
//    tvi.lParam = (LPARAM)nLevel; 
//    tvins.item = tvi; 
//    tvins.hInsertAfter = hPrev; 
//    // Set the parent item based on the specified level. 
//    if (nLevel == 1) 
//        tvins.hParent = TVI_ROOT; 
//    else if (nLevel == 2) 
//        tvins.hParent = hPrevRootItem; 
//    else 
//        tvins.hParent = hPrevLev2Item; 
//
//    // Add the item to the tree-view control. 
//    hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 
//
//    if (hPrev == NULL)
//        return NULL;
//
//    // Save the handle to the item. 
//    if (nLevel == 1) 
//        hPrevRootItem = hPrev; 
//    else if (nLevel == 2) 
//        hPrevLev2Item = hPrev; 
//
//    // The new item is a child item. Give the parent item a closed folder bitmap to indicate it now has child items. 
//    if (nLevel > 1) { 
//        hti = TreeView_GetParent(hwndTV, hPrev); 
//        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
//        tvi.hItem = hti; 
//        tvi.iImage = g_nClosed; 
//        tvi.iSelectedImage = g_nClosed; 
//        TreeView_SetItem(hwndTV, &tvi); 
//    } 
//
//    return hPrev; 
//} 
//
//// Extracts heading text and heading levels from a global 
//// array and passes them to a function that adds them as
//// parent and child items to a tree-view control. 
//// Returns TRUE if successful, or FALSE otherwise. 
//// hwndTV - handle to the tree-view control. 
//
//BOOL InitTreeViewItems(HWND hwndTV)
//{ 
//    HTREEITEM hti;
//
//    // g_rgDocHeadings is an application-defined global array of 
//    // the following structures: 
//    //     typedef struct 
//    //       { 
//    //         TCHAR tchHeading[MAX_HEADING_LEN]; 
//    //         int tchLevel; 
//    //     } Heading; 
//    for (int i = 0; i < ARRAYSIZE(g_rgDocHeadings); i++) 
//    { 
//        // Add the item to the tree-view control. 
//        hti = AddItemToTree(hwndTV, g_rgDocHeadings[i].tchHeading, 
//            g_rgDocHeadings[i].tchLevel); 
//
//        if (hti == NULL)
//            return FALSE;
//    } 
//           
//    return TRUE; 
//}
//HWND hd::CreateTreeView(HINSTANCE hInstance, HWND hwndParent) { 
//    RECT rcClient;  // dimensions of client area 
//    HWND hwndTV;    // handle to tree-view control 
//
//    // Get the dimensions of the parent window's client area, and create  the tree-view control. 
//    GetClientRect(hwndParent, &rcClient); 
//    hwndTV = CreateWindowEx(0, WC_TREEVIEW, TEXT("Tree View"), WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,  0,  0, rcClient.right, rcClient.bottom, hwndParent, (HMENU)ID_TREEVIEW, hInstance, 0); 
//
//    TVImages treeViewImages;
//    // Initialize the image list, and add items to the control. 
//    if(!InitTreeViewImageLists(hInstance, hwndTV, treeViewImages) || !InitTreeViewItems(hInstance, hwndTV)) { 
//        DestroyWindow(hwndTV); 
//        return FALSE; 
//    } 
//    return hwndTV;
//}