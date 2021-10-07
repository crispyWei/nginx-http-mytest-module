# mytest module for Nginx

## 說明

透過此開發模組練習，了解Nginx Http 模組運作

## 安裝

   1. 透過Configure新增此模組:
      > 
      >     動態模組: ./configure (...) --add-dynamic-module=/path/to/nginx-http-mytest-module
      >     
   2. 執行此命令 `make`
   3. 開始使用此模組，指令為 `mytest`
      
      ```
      Example:
          
          location = /test {
             
            mytest;
          
          }
      ```

## Http框架筆記

HTTP框架共有11階段，第三方只能介入其中7個階段處理請求。
```
typedef enum{
   在接收到完整的HTTP頭部後處理的HTTP階段
   1. NGX_HTTP_POST_READ_PHASE=0
   
   在還沒有查詢到URI匹配的location前，這時rewrite重寫URL也作為一個獨立的HTTP階段
   2. NGX_HTTP_SERVER_REWRITE_PHASE
   
   根據URI尋找匹配的location，這個階段通常由ngx_http_core_module模組實現，不建議其他HTTP模組重新定義這一階段的行為
   3. NGX_HTTP_FIND_CONFIG_PHASE
   
   在NGX_HTTP_FIND_CONFIG_PHASE階段之後重寫URL的意義與NGX_HTTP_SERVER_ REWRITE_PHASE階段顯然是不同的，因為這兩者會導致查找到不同的location塊（location是與URI進行匹配的）
   4. NGX_HTTP_REWRITE_PHASE
   
   這一階段是用於在rewrite重寫URL后重新跳到NGX_HTTP_FIND_CONFIG_PHASE階段，找到與新的URI匹配的location。 所以，這一階段是無法由第三方HTTP模塊處理的，而僅由ngx_http_core_module模組使用
   5. NGX_HTTP_POST_REWRITE_PHASE
   
   //處理NGX_HTTP_ACCESS_PHASE階段前，HTTP模組可以介入的處理階段
   6. NGX_HTTP_PREACCESS_PHASE
   
   //這個階段用於讓HTTP模組判斷是否允許這個請求訪問Nginx伺服器
   7. NGX_HTTP_ACCESS_PHASE
   
   //當NGX_HTTP_ACCESS_PHASE階段中HTTP模組的handler處理方法返回不允許訪問的錯誤碼時（實際是NGX_HTTP_FORBIDDEN或者NGX_HTTP_UNAUTHORIZED）， 這個階段將負責構造拒絕服務的用戶回應。 所以，這個階段實際上用於給NGX_HTTP_ACCESS_PHASE階段收尾
   8. NGX_HTTP_POST_ACCESS_PHASE
   
   //這個階段完全是為了try_files配置項而設立的。 當HTTP請求訪問靜態檔資源時，try_files配置項可以使這個請求順序地訪問多個靜態檔資源，如果某一次訪問失敗，則繼續訪問try_files中指定的下一個靜態資源。 另外，這個功能完全是在NGX_HTTP_TRY_FILES_PHASE階段中實現的
   9. NGX_HTTP_TRY_FILES_PHASE
   
   //用於處理HTTP請求內容的階段，這是大部分HTTP模組最喜歡介入的階段
   10. NGX_HTTP_CONTENT_PHASE

   處理完請求後記錄日誌的階段。 例如，ngx_http_log_module模組就在這個階段中加入了一個handler處理方法，使得每個HTTP請求處理完畢後會記錄access_log日誌
   11. NGX_HTTP_LOG_PHASE

} ngx_http_phases;

```
