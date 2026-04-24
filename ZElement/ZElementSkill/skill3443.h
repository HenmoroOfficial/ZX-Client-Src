#ifndef __CPPGEN_GNET_SKILL3443
#define __CPPGEN_GNET_SKILL3443
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3443:public Skill
    {
      public:
        enum
        { SKILL_ID = 3443 };
          Skill3443 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3443Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3443Stub ():SkillStub (3443)
        {
            occupation = 67;
            name = L"»ğÑæÎÆÕÂ";
            nativename = "»ğÑæÎÆÕÂ";
            icon = "»ğÑæÎÆÕÂ.dds";
            maxlevel = 20;
            maxlearn = 3;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 3386;
            talent[1] = 3388;
            talent[2] = 3748;
            talent_size = 3;
            action[0] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[1] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[2] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[3] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[4] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[5] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[6] = "·ÙÏã¹È_»ğÑæÎÆÕÂ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[16] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[17] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[18] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[19] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[20] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ğÑæÎÆÕÂ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3443Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3442 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (720 + 10 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 150000 - 150000 * 0.04 * skill->GetT2 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 110, 115, 120 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               720 + 10 * skill->GetLevel (), 6 * skill->GetLevel (), (40 + 20 * skill->GetLevel ()), 5 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (6 * skill->GetLevel () * 1000 + skill->GetT0 () * 5 * 1000);
            skill->GetVictim ()->SetRatio ((40 + 20 * skill->GetLevel () + 20 * skill->GetT0 ()) * 0.01);
            skill->GetVictim ()->SetNingjin (1);
            skill->GetVictim ()->SetTime (6 * skill->GetLevel () * 1000 *
                                          int (skill->GetPlayer ()->GetMp () > skill->GetPlayer ()->GetMaxmp () * 0.95));
              skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetCrit () * 0.001 * (5 * skill->GetLevel () + 5 * skill->GetT1 ()) * 0.01);
              skill->GetVictim ()->SetInccritrate (1);
              skill->GetVictim ()->SetProbability (1.0 * (skill->GetT2 () > 0 ? (5 + 2 * int (skill->GetLevel () / 2)) : 0));
              skill->GetVictim ()->SetTime (6 * skill->GetLevel () * 1000 *
                                            int (skill->GetPlayer ()->GetMp () > skill->GetPlayer ()->GetMaxmp () * 0.95));
              skill->GetVictim ()->SetRatio (int (skill->GetPlayer ()->GetMp () / 10000) * 0.005001 * skill->GetT2 ());
              skill->GetVictim ()->SetBuffid (5);
              skill->GetVictim ()->SetInccrithurt (1);
              skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasbuff (4141) == 1 ? 120 : 0));
              skill->GetVictim ()->SetRatio (4141);
              skill->GetVictim ()->SetAmount (4141);
              skill->GetVictim ()->SetValue (4141);
              skill->GetVictim ()->SetDispel (1);
              return true;
        }
#endif
    };
}
#endif
