#include "Modify.h"

#include <algorithm>


HRESULT Page::Modify::DocumentLocation(BSTR &docLocation, IHTMLDocument2* pHtmlDoc2)
{
	if (pHtmlDoc2 == NULL)
	{
		Debug::Error("DocumentLocation: pHtmlDoc2 is NULL");
		return E_FAIL;
	}

	if (docLocation == NULL)
	{
		Debug::Warn("docLocation is NULL");
		return E_FAIL;
	}

	try
	{
		CComPtr<IHTMLLocation> pLocation = NULL;
		HRESULT hr = pHtmlDoc2->get_location(&pLocation);
		if (FAILED(hr) || pLocation == NULL)
		{
			Debug::Error("Could not retrieve document location");
			return E_FAIL;
		}
		else
		{
			hr = pLocation->get_href(&docLocation);
			if(FAILED(hr) || docLocation == NULL)
			{
				Debug::Error("Could not get href from location");
				return E_FAIL;
			}
			else
			{
				Debug::Log("Doc location: %ls", docLocation);
				return S_OK;
			}
		}
	}
	catch(std::exception e)
	{
		Debug::Error(e.what());
		return E_FAIL;
	}
}

BOOL Page::Modify::IsHTTPS(BSTR location)
{
	if (location == NULL)
	{
		Debug::Error("location is NULL in IsHTTPS");
		return FALSE;
	}

	try
	{
		std::wstring wUrl(location);
		transform(wUrl.begin(), wUrl.end(), wUrl.begin(), tolower);

		std::wstring schemeUrl = wUrl.substr(0, 5);
		Debug::Log("main URL scheme: %ls", schemeUrl.c_str());

		size_t convertedChars = 0;
		std::vector<wchar_t> dest(10);
		mbstowcs_s(&convertedChars, &dest[0], 5, "https", 10);
		std::wstring https(&dest[0]);

		//Debug::Log("%ls", https.c_str());

		if (schemeUrl != https)
		{
			Debug::Log("Main URL is http://");
			return FALSE;
		}

		return TRUE;
	}
	catch(std::exception e)
	{
		Debug::Error(e.what());
		return FALSE;
	}
}

HRESULT Page::Modify::UpdateHead(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules)
{
	if (pHtmlDoc2 == NULL)
	{
		Debug::Error("UpdateHead: pHtmlDoc2 is NULL");
		return E_FAIL;
	}

	try
	{
		// Get <head>
		CComPtr<IHTMLElementCollection> pColl = NULL;
		HRESULT hr = pHtmlDoc2->get_all( &pColl );
		if(FAILED(hr) || pColl == NULL)
		{
			Debug::Error("All tags NOT found");
			return E_FAIL;
		}
		//Debug::Log("tags found");

		LONG celem;
		hr = pColl->get_length( &celem );
		if (FAILED(hr))
		{
			Debug::Error("tags length NOT found");
			return E_FAIL;
		}
		//Debug::Log("Number of tags: %i", celem);

		CComPtr<IHTMLHeadElement> pHead = NULL;

		for (long i=0; i< celem; i++)
		{
			VARIANT varIndex;
			varIndex.vt = VT_UINT;
			varIndex.lVal = i;
			VARIANT var2;
			VariantInit( &var2 );
			CComPtr<IDispatch> spDisp;
			hr = pColl->item( varIndex, var2, &spDisp );
			if ( hr == S_OK )
			{
				//Debug::Log("Analyze tag %i", i);
				CComQIPtr<IHTMLElement> pElem(spDisp);
				if (! pElem )
				{
					Debug::Warn("Could not get IHTMLElement");
					continue;
				}

				BSTR bstr = NULL;
				hr = pElem->get_tagName(&bstr);
				//Debug::Log("tag name: %ls", bstr);

				if(bstr == NULL)
				{
					Debug::Error("UpdateHead: tag name not found");
					continue;
				}
				
				//Is it <head>

				hr = spDisp->QueryInterface(IID_IHTMLHeadElement,(void **)&pHead );
				if (SUCCEEDED(hr) && pHead != NULL)
				{
					Debug::Log("HEAD element found");
					// break from loop
					break;
				}
				else
				{
					Debug::Warn("head NOT found");
					return E_FAIL;
				}
			}
		}


		if (pHead == NULL)
		{
			Debug::Warn("head NOT found");
			return E_FAIL;
		}

		Debug::Log("HEAD found");

		CComQIPtr<IHTMLDOMNode> spNode(pHead);
		if(! spNode)
		{
			Debug::Warn("Could not get node from HEAD");
			return E_FAIL;
		}

		return UpdateDOM(spNode, FALSE, rules);
	}
	catch(std::exception e)
	{
		Debug::Error(e.what());
		return E_FAIL;
	}
}


HRESULT Page::Modify::UpdateBody(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules)
{
	if (pHtmlDoc2 == NULL)
	{
		Debug::Error("UpdateBody: pHtmlDoc2 is NULL");
		return E_FAIL;
	}

	try
	{
		// Get <body>
		CComPtr<IHTMLElement> pBody = NULL;
		HRESULT hr = pHtmlDoc2->get_body(&pBody);

		if(FAILED(hr) || pBody == NULL)
		{
			Debug::Log("body NOT found");
			return E_FAIL;
		}

		Debug::Log("BODY found");

		CComQIPtr<IHTMLDOMNode> spNode(pBody);
		if(! spNode)
		{
			Debug::Warn("Could not get node from BODY");
			return E_FAIL;
		}

		return UpdateDOM(spNode, TRUE, rules);
	}
	catch(std::exception e)
	{
		Debug::Error(e.what());
		return E_FAIL;
	}
}

HRESULT Page::Modify::UpdateDOM(IHTMLDOMNode* spNode, BOOL recursive, HTTPS::Rules &rules)
{

	if (spNode == NULL)
	{
		Debug::Error("UpdateDOM: spNode is NULL");
		return E_FAIL;
	}

	CComPtr<IDispatch> spCollectionDispatch = NULL; 
	HRESULT hr = spNode->get_childNodes(&spCollectionDispatch);
	if(FAILED(hr) || spCollectionDispatch == NULL)
	{
		Debug::Warn("Could not get all nodes from spNode");
		return E_FAIL;
	}


	CComQIPtr<IHTMLDOMChildrenCollection> spCollection(spCollectionDispatch); 
	if (! spCollection)
	{
		Debug::Warn("Could not cast spCollectionDispatch to IHTMLDOMChildrenCollection");
		return E_FAIL;
	}

	// Need bettter way to compare scheme with https
	std::wstring https(L"https");
	//Debug::Log("TEST: %ls", https.c_str());

	long lCount = 0;
	spCollection->get_length(&lCount);
	//Debug::Log("Children nodes: %i", lCount );

	for(long i = 0; i < lCount ; i++)
	{
		CComPtr<IDispatch> spChild = NULL;;
		spCollection->item(i, &spChild);

		if(! spChild || spChild == NULL)
		{
			Debug::Warn("spChild not found");
			continue;
		}

		CComQIPtr<IHTMLDOMNode> pChild(spChild);
		if(! pChild)
		{
			Debug::Warn("Could not cast IHTMLDOMNode");
			continue;
		}

		BSTR name = NULL;
		pChild->get_nodeName(&name);
		if(name == NULL)
		{
			Debug::Warn("Could not get node name");
			continue;
		}

		//Debug::Log("Node name: %ls (%i)", name, i);
		if(0 != wcscmp(name, L"#text") && 0 != wcscmp(name, L"#comment"))
		{
			//Debug::Log("Tag found");

			if(recursive)
			{
				//Debug::Log("Recursion...");
				UpdateDOM(pChild, recursive, rules);
			}

			CComPtr<IDispatch> spAttributes = NULL;
			pChild->get_attributes(&spAttributes);
			if (spAttributes == NULL)
			{
				//Debug::Log("no attribute?");
				continue;
			}
			//Debug::Log("Tag found 1");

			CComQIPtr<IHTMLAttributeCollection> pAcoll(spAttributes);
			if (! pAcoll)
			{
				//Debug::Log("no attributes");
				continue;
			}
			//Debug::Log("Tag found 2");

			long lLength = 0;
			hr = pAcoll->get_length(&lLength);
			//Debug::Log("number of attributes: %i", lLength);
			if (SUCCEEDED(hr) && lLength > 0)
			{
				VARIANT vACIndex;
				vACIndex.vt = VT_I4;
				BSTR bstrName;
				_variant_t vValue;
				VARIANT_BOOL vbSpecified;

				CComPtr<IDispatch> pItemDisp = NULL;
				
				for(long j = 0; j < lLength ; j++)
				{
					vACIndex.lVal = j;
					hr = pAcoll->item(&vACIndex, &pItemDisp);
					if(FAILED(hr) || pItemDisp == NULL)
						continue;

					 CComQIPtr<IHTMLDOMAttribute> pItem(pItemDisp);
					 if(! pItem)
						 continue;

					 hr = pItem->get_specified(&vbSpecified);
					 if(FAILED(hr))
					   continue;

					 if(vbSpecified)
					 {
						hr = pItem->get_nodeName(&bstrName);
						if(FAILED(hr))
							continue;
						//Debug::Log("Tag attribute name: %ls", bstrName);

						hr = pItem->get_nodeValue(&vValue);
						if(FAILED(hr))
						{
							Debug::Warn("Could not get value");
							continue;
						}
						if (vValue.vt != NULL && (wcscmp(bstrName, L"href") == 0 || wcscmp(bstrName, L"src") == 0))
						{
							//Debug::Log("attribute value: %ls", vValue.GetVARIANT().bstrVal);

							std::wstring wHref(vValue.GetVARIANT().bstrVal);
							transform(wHref.begin(), wHref.end(), wHref.begin(), tolower);


							if (wHref.length() < 5)
							{
								Debug::Log("whref too short, skip");
								continue;
							}

							std::wstring schemeHref = wHref.substr(0, 5);

							//Debug::Log("tag: %ls", schemeHref.c_str());
							if(schemeHref == https)
							{
								//Debug::Log("Scheme is already HTTPS");
							}
							else
							{
								std::string sHref(wHref.begin(), wHref.end()); // Transform WCHAR into CHAR, safe???
								char* newUrl = rules.Transform(const_cast<char*>(sHref.c_str()));

								if (newUrl != NULL)
								{
									Debug::Log("Base needs to be updated to %s", newUrl);

									CComQIPtr<IHTMLElement> newBase(pChild);
									if (! newBase)
									{
										Debug::Error("Coud not create newBase");
										continue;
									}

									CComBSTR ccombstr(newUrl);
									VARIANT varValue;
									VariantInit(&varValue);
									V_VT(&varValue)=VT_BSTR;
									V_BSTR(&varValue) = ccombstr;
									newBase->setAttribute(bstrName, varValue, 0);
									//Debug::Log("Node updated");

									//SysFreeString(ccombstr); // Not beed with CComBSTR: http://msdn.microsoft.com/en-us/library/bdyd6xz6%28v=vs.80%29.aspx
									delete newUrl;
								}
							}
						}
					 }
				}
			}
			//break;
		}

		if(name != NULL)
			SysFreeString(name);
	}
	
	return S_OK;
}


BOOL Page::Modify::ShouldModify(IHTMLDocument2* pHtmlDoc2, HTTPS::Rules &rules)
{
	if (pHtmlDoc2 == NULL)
	{
		Debug::Error("ShouldModify: pHtmlDoc2 is NULL");
		return E_FAIL;
	}

	try
	{
		BSTR docLocation = NULL;
		HRESULT hr = Page::Modify::DocumentLocation(docLocation, pHtmlDoc2);
		if(FAILED(hr) || docLocation == NULL)
		{
			Debug::Error("Could not retrieve document location");
			return FALSE;
		}


		// Page is HTTPS:// ?
		if (Page::Modify::IsHTTPS(docLocation) == FALSE)
		{
			Debug::Log("Main URL is http://, skip");
			SysFreeString(docLocation);
			return FALSE;
		}

		// Rule match on the host?
		char* url = _com_util::ConvertBSTRToString(docLocation);
		SysFreeString(docLocation);
		if (rules.HostMatchFromUrl(url) == false)
		{
			Debug::Log("No host match for %s", url);
			delete url;
			return FALSE;
		}

		delete url;
		return TRUE;
	}
	catch(std::exception e)
	{
		Debug::Error(e.what());
		return FALSE;
	}
}