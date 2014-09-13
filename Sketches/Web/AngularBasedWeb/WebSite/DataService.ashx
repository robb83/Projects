<%@ WebHandler Language="C#" Class="DataService" %>

using System;
using System.IO;
using System.Web;
using System.Reflection;
using System.Collections.Generic;
using Newtonsoft.Json;

// demo purposes only

public class DataService : IHttpHandler {

    static Dictionary<String, Tuple<Func<object, object>, Type, Type>> functions = new Dictionary<String, Tuple<Func<object, object>, Type, Type>> {
        { "Authentication", new Tuple<Func<object, object>, Type, Type>(r => (object)Authentication((AuthenticationRequest)r), typeof(AuthenticationRequest), typeof(AuthenticationResponse)) },
        { "ForgottenPassword", new Tuple<Func<object, object>, Type, Type>(r => (object)ForgottenPassword((ForgottenPasswordRequest)r), typeof(ForgottenPasswordRequest), typeof(ForgottenPasswordResponse)) },
        { "Products", new Tuple<Func<object, object>, Type, Type>(r => (object)ProductList((ProductListRequest)r), typeof(ProductListRequest), typeof(ProductListResponse)) },
        { "ProductDetail", new Tuple<Func<object, object>, Type, Type>(r => (object)ProductDetail((ProductDetailRequest)r), typeof(ProductDetailRequest), typeof(ProductDetailResponse)) },
        { "ChangePassword", new Tuple<Func<object, object>, Type, Type>(r => (object)ChangePassword((ChangePasswordRequest)r), typeof(ChangePasswordRequest), typeof(ChangePasswordResponse)) },
    };

    public class BaseResponse
    {
        public String ErrorDescription { get; set; }
        public bool Success { get; set; }
    }

    public class ChangePasswordRequest
    {
        public String Username { get; set; }
        public String OldPassword { get; set; }
        public String NewPassword { get; set; }
    }

    public class ChangePasswordResponse : BaseResponse
    {
    }

    public class ProductListInfo
    {
        public int ID { get; set; }
        public String Name { get; set; }
        public decimal Price { get; set; }
        public String Currency { get; set; }
        public DateTime Created { get; set; }
    }

    public class ProductDetailInfo
    {
        public int ID { get; set; }
        public String Name { get; set; }
        public decimal Price { get; set; }
        public String Currency { get; set; }
        public DateTime Created { get; set; }
    }

    public class ProductDetailResponse : BaseResponse
    {
        public ProductDetailInfo Product { get; set; }
    }

    public class ProductDetailRequest
    {
        public int ProductID { get; set; }
    }

    public class ProductListResponse : BaseResponse
    {
        public List<ProductListInfo> Products { get; set; }
        public int TotalCount { get; set; }
    }

    public class ProductListRequest
    {
        public int PageNumber { get; set; }
        public int PageSize { get; set; }
        public String Search { get; set; }
    }

    public class ForgottenPasswordResponse : BaseResponse
    {
        public String SuccessDescription { get; set; }
    }

    public class ForgottenPasswordRequest
    {
        public String Email { get; set; }
    }

    public class AuthenticationResponse : BaseResponse
    {
        public String Username { get; set; }
        public String Token { get; set; }
    }

    public class AuthenticationRequest
    {
        public String Username { get; set; }
        public String Password { get; set; }
    }

    public static ChangePasswordResponse ChangePassword(ChangePasswordRequest request)
    {
        ChangePasswordResponse response = new ChangePasswordResponse();

        if (request == null)
        {
            ThrowInvalidRequest();
            return null;
        }

        if (String.IsNullOrWhiteSpace(request.Username))
        {
            response.Success = false;
            response.ErrorDescription = "Missing Username.";
        }
        else if (String.IsNullOrWhiteSpace(request.OldPassword))
        {
            response.Success = false;
            response.ErrorDescription = "Missing Old Password.";
        }
        else if (String.IsNullOrWhiteSpace(request.NewPassword))
        {
            response.Success = false;
            response.ErrorDescription = "Missing New Password.";
        }
        else if (request.NewPassword.Length < 2)
        {
            response.Success = false;
            response.ErrorDescription = "New Password Is Too Short.";
        }
        else
        {
            response.Success = true;
        }

        return response;
    }

    public static ProductDetailResponse ProductDetail(ProductDetailRequest request)
    {
        ProductDetailResponse response = new ProductDetailResponse();

        if (request == null)
        {
            ThrowInvalidRequest();
            return null;
        }

        response.Success = true;
        response.Product = new ProductDetailInfo
        {
            ID = request.ProductID,
            Created = DateTime.Now,
            Name = "Product #" + (request.ProductID),
            Price = 10000,
            Currency = "HUF"
        };

        return response;
    }

    public static ProductListResponse ProductList(ProductListRequest request)
    {
        ProductListResponse response = new ProductListResponse();

        if (request == null)
        {
            ThrowInvalidRequest();
            return null;
        }

        const int totalCount = 1005;

        int skiped = (request.PageNumber - 1) * request.PageSize;
        int productCountInResponse = totalCount - skiped;
        if (productCountInResponse < 0)
            productCountInResponse = 0;
        if (productCountInResponse > request.PageSize)
            productCountInResponse = request.PageSize;

        response.TotalCount = totalCount;
        response.Products = new List<ProductListInfo>();
        response.Success = true;

        for (int p = 0; p < productCountInResponse; ++p)
        {
            response.Products.Add(new ProductListInfo
            {
                ID = skiped + p,
                Created = DateTime.Now,
                Name = "Product #" + ( skiped + p),
                Price = 10000,
                Currency = "HUF"
            });
        }

        return response;
    }

    public static ForgottenPasswordResponse ForgottenPassword(ForgottenPasswordRequest request)
    {
        ForgottenPasswordResponse response = new ForgottenPasswordResponse();

        if (request == null)
        {
            ThrowInvalidRequest();
            return null;
        }

        if (!String.IsNullOrWhiteSpace(request.Email))
        {
            response.Success = false;
            response.ErrorDescription = "Missing username";
        }
        else
        {
            response.Success = true;
            response.SuccessDescription = "New password request email sended.";
        }

        return response;
    }

    public static AuthenticationResponse Authentication(AuthenticationRequest request)
    {
        AuthenticationResponse response = new AuthenticationResponse();

        if (request == null)
        {
            ThrowInvalidRequest();
            return null;
        }

        if (String.IsNullOrWhiteSpace(request.Username)
            || String.IsNullOrWhiteSpace(request.Password)
            || request.Password.Length < 2)
        {
            response.Success = false;
            response.ErrorDescription = "Missing username/password";
        }
        else
        {
            response.Success = true;
            response.Username = request.Username;
            response.Token = request.Username;
        }

        return response;
    }

    public void ProcessRequest (HttpContext context) {

        HttpRequest request = context.Request;

        // check request condition
        if (String.IsNullOrWhiteSpace(request.ContentType)
            || !request.ContentType.StartsWith("application/json")
            || request.HttpMethod != "POST")
        {
            throw new HttpException(400, "Invalid request");
        }

        // check function exists
        String functionName = request.PathInfo;

        if (String.IsNullOrWhiteSpace(functionName))
        {
            throw new HttpException(400, "Invalid request");
        }

        functionName = functionName.Substring(1);

        Tuple<Func<object, object>, Type, Type> functionInfo;

        if (!functions.TryGetValue(functionName, out functionInfo))
        {
            throw new HttpException(404, "Not Found");
        }

        var serializer = JsonSerializer.CreateDefault();

        object functionInput;
        using(JsonReader reader = new JsonTextReader(new StreamReader(context.Request.InputStream)))
        {
            functionInput = serializer.Deserialize(reader, functionInfo.Item2);
        }

        object functionOutput = functionInfo.Item1.Invoke(functionInput);

        context.Response.ContentType = "application/json";
        using(JsonWriter writer = new JsonTextWriter(new StreamWriter(context.Response.OutputStream)))
        {
            serializer.Serialize(writer, functionOutput);
        }
    }

    public static void ThrowInvalidRequest()
    {
        throw new HttpException(400, "Invalid request");
    }

    public bool IsReusable {
        get {
            return true;
        }
    }

}